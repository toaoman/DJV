//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------s

#include <djvViewLib/FileCache.h>

#include <djvViewLib/Context.h>
#include <djvViewLib/FilePrefs.h>

#include <djvGraphics/Image.h>

#include <djvCore/Assert.h>
#include <djvCore/ListUtil.h>
#include <djvCore/Memory.h>

#include <algorithm>

namespace djv
{
    namespace ViewLib
    {
        struct FileCacheItem::Private
        {
            Private(Graphics::Image * in, const void * key, qint64 frame) :
                image(in),
                key(key),
                frame(frame),
                count(1)
            {}

            QScopedPointer<Graphics::Image> image;
            const void *                    key;
            qint64                          frame;
            int                             count;
        };

        namespace
        {
            int refAliveCount = 0;

        } // namespace

        FileCacheItem::FileCacheItem(
            Graphics::Image * image,
            const void *      key,
            qint64            frame) :
            _p(new Private(image, key, frame))
        {
            ++refAliveCount;

            //DJV_DEBUG("FileCacheItem::FileCacheItem");
            //DJV_DEBUG_PRINT("alive = " << refAliveCount);
        }

        FileCacheItem::~FileCacheItem()
        {
            --refAliveCount;

            //DJV_DEBUG("FileCacheItem::~FileCacheItem");
            //DJV_DEBUG_PRINT("alive = " << refAliveCount);
        }

        Graphics::Image * FileCacheItem::image()
        {
            return _p->image.data();
        }

        const void * FileCacheItem::key() const
        {
            return _p->key;
        }

        void FileCacheItem::resetKey()
        {
            _p->key = 0;
        }

        qint64 FileCacheItem::frame() const
        {
            return _p->frame;
        }

        void FileCacheItem::increment()
        {
            ++_p->count;
        }

        void FileCacheItem::decrement()
        {
            --_p->count;
        }

        int FileCacheItem::count() const
        {
            return _p->count;
        }

        struct FileCache::Private
        {
            Private(Context * context) :
                maxByteCount(static_cast<quint64>(context->filePrefs()->cacheSize() * Core::Memory::gigabyte)),
                context(context)
            {}

            FileCacheItemList items;
            quint64           maxByteCount = 0;
            quint64           cacheByteCount = 0;
            Context *         context = nullptr;
        };

        FileCache::FileCache(Context * context, QObject * parent) :
            QObject(parent),
            _p(new Private(context))
        {
            //DJV_DEBUG("FileCache::FileCache");

            connect(
                context->filePrefs(),
                SIGNAL(cacheChanged(bool)),
                SLOT(cacheCallback(bool)));
            connect(
                context->filePrefs(),
                SIGNAL(cacheSizeChanged(float)),
                SLOT(cacheSizeCallback(float)));
        }

        FileCache::~FileCache()
        {
            //DJV_DEBUG("FileCache::~FileCache");

            //debug();

            // Cleanup.
            const int count = _p->items.count();
            for (int i = 0; i < count; ++i)
            {
                delete _p->items[i];
            }
        }

        FileCacheItem * FileCache::create(
            Graphics::Image * image,
            const void *      key,
            qint64            frame)
        {
            //DJV_DEBUG("FileCache::create");
            //DJV_DEBUG_PRINT("image = " << *image);
            //DJV_DEBUG_PRINT("key = " << reinterpret_cast<qint64>(key));
            //DJV_DEBUG_PRINT("frame = " << frame);

            //debug();

            // Create a new reference.
            FileCacheItem * out = new FileCacheItem(image, key, frame);

            // Add the reference to the end of the list.
            _p->items += out;

            // Update the cache size.
            _p->cacheByteCount += out->image()->dataByteCount();
            if (_p->cacheByteCount > _p->maxByteCount)
            {
                purge();
            }
            Q_EMIT cacheChanged();
            //debug();

            return out;
        }

        FileCacheItem * FileCache::get(const void * key, qint64 frame)
        {
            //DJV_DEBUG("FileCache::get");
            //DJV_DEBUG_PRINT("key = " << reinterpret_cast<qint64>(key));
            //DJV_DEBUG_PRINT("frame = " << frame);

            //debug();

            // Search for the requested item.
            FileCacheItem * item = 0;
            const int count = _p->items.count();
            for (int i = 0; i < count; ++i)
            {
                if (key == _p->items[i]->key() && frame == _p->items[i]->frame())
                {
                    // Found it.
                    item = _p->items[i];

                    // Increment the reference count.
                    item->increment();

                    // Move the item to the end of the list.
                    _p->items.remove(i);
                    _p->items.push_back(item);

                    break;
                }
            }
            //debug();

            //DJV_DEBUG_PRINT("out = " <<
            //  (out ? reinterpret_cast<qint64>(out->key()) : 0));
            return item;
        }

        bool FileCache::contains(const void * key, qint64 frame) const
        {
            const int count = _p->items.count();
            for (int i = 0; i < count; ++i)
            {
                if (key == _p->items[i]->key() && frame == _p->items[i]->frame())
                {
                    return true;
                }
            }
            return false;
        }

        void FileCache::del(const void * key)
        {
            //DJV_DEBUG("FileCache::del");
            //DJV_DEBUG_PRINT("key = " << reinterpret_cast<qint64>(key));

            debug();

            // Search for all items that match the given key.
            for (int i = _p->items.count() - 1; i >= 0; --i)
            {
                FileCacheItem * item = _p->items[i];
                if (key == item->key())
                {
                    // If the item has a reference count of zero we can delete it now,
                    // otherwise we will have to wait and delete it later.
                    if (!item->count())
                    {
                        removeItem(i);
                    }
                    else
                    {
                        item->resetKey();
                    }
                }
            }

            // Emit a signal that the cache has changed.
            Q_EMIT cacheChanged();

            debug();
        }

        void FileCache::del(const void * key, qint64 frame)
        {
            //DJV_DEBUG("FileCache::del");
            //DJV_DEBUG_PRINT("key = " << reinterpret_cast<qint64>(key));
            //DJV_DEBUG_PRINT("frame = " << frame);

            debug();

            // Search for all items that match the given key and frame.
            for (int i = _p->items.count() - 1; i >= 0; --i)
            {
                FileCacheItem * item = _p->items[i];
                if (key == item->key() && frame == item->frame())
                {
                    // If the item has a reference count of zero we can delete it now,
                    // otherwise we will have to wait and delete it later.
                    if (!item->count())
                    {
                        removeItem(i);
                    }
                    else
                    {
                        item->resetKey();
                    }
                }
            }

            // Emit a signal that the cache has changed.
            Q_EMIT cacheChanged();

            debug();
        }

        void FileCache::clear()
        {
            //DJV_DEBUG("FileCache::clear");

            debug();

            // Delete all the items with a reference count of zero.
            for (int i = _p->items.count() - 1; i >= 0; --i)
            {
                if (!_p->items[i]->count())
                {
                    removeItem(i);
                }
            }

            // Emit a signal that the cache has changed.
            Q_EMIT cacheChanged();

            debug();
        }

        FileCacheItemList FileCache::items(const void * key)
        {
            FileCacheItemList items;
            const int count = _p->items.count();
            for (int i = 0; i < count; ++i)
            {
                if (key == _p->items[i]->key())
                {
                    items.append(_p->items[i]);
                }
            }
            return items;
        }

        namespace
        {
            bool compare(qint64 a, qint64 b)
            {
                return a < b;
            }

        } // namespace

        Core::FrameList FileCache::frames(const void * key)
        {
            Core::FrameList frames;
            const int count = _p->items.count();
            for (int i = 0; i < count; ++i)
            {
                if (key == _p->items[i]->key())
                {
                    frames.append(_p->items[i]->frame());
                }
            }
            qSort(frames.begin(), frames.end(), compare);
            return frames;
        }

        float FileCache::maxSize() const
        {
            return _p->maxByteCount / static_cast<float>(Core::Memory::gigabyte);
        }

        quint64 FileCache::maxByteCount() const
        {
            return _p->maxByteCount;
        }

        float FileCache::size(const void * key) const
        {
            quint64 size = 0;
            const int count = _p->items.count();
            for (int i = 0; i < count; ++i)
            {
                if (key == _p->items[i]->key())
                {
                    size += _p->items[i]->image()->dataByteCount();
                }
            }
            return size / static_cast<float>(Core::Memory::gigabyte);
        }

        float FileCache::size() const
        {
            return _p->cacheByteCount / static_cast<float>(Core::Memory::gigabyte);
        }

        quint64 FileCache::byteCount() const
        {
            return _p->cacheByteCount;
        }

        const QVector<float> & FileCache::sizeDefaults()
        {
            static const QVector<float> data = QVector<float>() <<
                1.f <<
                2.f <<
                3.f <<
                6.f <<
                12.f <<
                24.f <<
                48.f <<
                96.f;
            return data;
        }

        const QStringList & FileCache::sizeLabels()
        {
            static const QStringList data = QStringList() <<
                "1" <<
                "2" <<
                "3" <<
                "6" <<
                "12" <<
                "24" <<
                "48" <<
                "96";
            return data;
        }

        void FileCache::debug()
        {
            /*DJV_DEBUG("FileCache::debug");
            DJV_DEBUG_PRINT("items = " << _p->items.count());
            DJV_DEBUG_PRINT("cache max = " << maxSize());
            DJV_DEBUG_PRINT("cache size = " << size());

            for (int i = 0; i < _p->items.count(); ++i)
            {
                DJV_DEBUG_PRINT(
                    "item (count = " << _p->items[i]->count() << ") = " <<
                    reinterpret_cast<qint64>(_p->items[i]->key()) << " " <<
                    _p->items[i]->frame());
            }*/
        }

        void FileCache::setMaxSize(float size)
        {
            //DJV_DEBUG("FileCache::setMaxSize");
            //DJV_DEBUG_PRINT("size = " << size);
            //debug();
            _p->maxByteCount = static_cast<quint64>(size * Core::Memory::gigabyte);
            //if (_p->cacheByteCount > _p->maxByteCount)
            purge();
            //debug();
        }

        void FileCache::removeItem(int index)
        {
            FileCacheItem * item = _p->items[index];
            _p->items.remove(index);
            _p->cacheByteCount -= item->image()->dataByteCount();
            delete item;
        }

        void FileCache::purge()
        {
            //DJV_DEBUG("FileCache::purge");

            debug();

            // Delete as many items as possible to bring the cache size below
            // the maximum size.
            QVector<int> itemsToDelete;
            const int count = _p->items.count();
            quint64 byteCountTmp = _p->cacheByteCount;
            for (int i = 0; i < count; ++i)
            {
                if (!_p->items[i]->count() && byteCountTmp > _p->maxByteCount)
                {
                    itemsToDelete += i;
                    byteCountTmp -= _p->items[i]->image()->dataByteCount();
                }
            }
            //DJV_DEBUG_PRINT("zombies = " << zombies.count());

            for (int i = itemsToDelete.count() - 1; i >= 0; --i)
            {
                removeItem(itemsToDelete[i]);
            }

            // Emit a signal that the cache has changed.
            Q_EMIT cacheChanged();

            debug();
        }

        void FileCache::cacheCallback(bool cache)
        {
            if (!cache)
            {
                clear();
            }
        }

        void FileCache::cacheSizeCallback(float size)
        {
            setMaxSize(size);
        }

    } // namespace ViewLib
} // namespace djv