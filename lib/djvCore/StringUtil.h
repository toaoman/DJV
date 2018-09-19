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
//------------------------------------------------------------------------------

#pragma once

#include <djvConfig.h>

#include <QString>
#include <QVector>

namespace djv
{
    namespace Core
    {
        //! \class StringUtil
        //!
        //! This class provides string utilities.
        class StringUtil
        {
        public:
            virtual ~StringUtil() = 0;

            //! Find a string in a list.
            static inline bool find(
                const QString &     string,
                const QStringList & list,
                int *               index = 0);

            //! Find a string in a list, case insensitive.
            static inline bool findNoCase(
                const QString &     string,
                const QStringList & list,
                int *               index = 0);

            //! Add double quotes to each string in a list.
            static QStringList addQuotes(const QStringList &);

            //! Get a label for a value.
            template<typename T>
            static inline QStringList label(const T &);

            //! Get the boolean labels.
            static const QStringList & boolLabels();

            //! C-string conversion.
            static int cString(
                const QString & string,
                char *          cString,
                int             maxLen = 0,
                bool            terminate = true);

            //! Default C-string length.
            static const int cStringLength = 256;

            //! Convert a string to an integer.
            template<typename T>
            static inline T stringToInt(
                const char * string,
                int          maxLen = cStringLength);

            //! Convert a string to an unsigned integer.
            template<typename T>
            static inline T stringToUInt(
                const char * string,
                int          maxLen = cStringLength);

            //! Convert an integer to a string.
            template<typename T>
            static inline int intToString(
                T      value,
                char * string,
                int    maxLen = cStringLength);
        };

#if defined(DJV_WINDOWS)
#define SNPRINTF ::sprintf_s
#else // DJV_WINDOWS
#define SNPRINTF ::snprintf
#endif // DJV_WINDOWS

        bool Serialize(QStringList &, QString &);
        bool Serialize(QStringList &, unsigned int &, const QStringList & labels);
        template <typename T>
        inline bool Serialize(QStringList &, T &, const QStringList & labels);
        template <typename T>
        inline QStringList Serialize(T, const QStringList & labels);

    } // namespace Core

    QStringList & operator >> (QStringList &, QString &) throw (QString);
    QStringList & operator >> (QStringList &, QStringList &) throw (QString);
    QStringList & operator >> (QStringList &, bool &) throw (QString);
    QStringList & operator >> (QStringList &, int &) throw (QString);
    QStringList & operator >> (QStringList &, unsigned int &) throw (QString);
    QStringList & operator >> (QStringList &, qint64 &) throw (QString);
    QStringList & operator >> (QStringList &, quint64 &) throw (QString);
    QStringList & operator >> (QStringList &, float &) throw (QString);
    QStringList & operator >> (QStringList &, double &) throw (QString);
    QStringList & operator >> (QStringList &, QVector<bool> &) throw (QString);
    //template <typename T>
    //inline QStringList & operator >> (QStringList &, QVector<T> &) throw (QString);

    QStringList & operator << (QStringList &, const char *);
    QStringList & operator << (QStringList &, bool);
    QStringList & operator << (QStringList &, int);
    QStringList & operator << (QStringList &, unsigned int);
    QStringList & operator << (QStringList &, qint64);
    QStringList & operator << (QStringList &, quint64);
    QStringList & operator << (QStringList &, float);
    QStringList & operator << (QStringList &, double);
    QStringList & operator << (QStringList &, const QVector<bool>&);
    //template <typename T>
    //inline QStringList & operator << (QStringList &, const QVector<T> &);

    //! This macros provides string serialize operators.
#define DJV_STRING_OPERATOR(TYPE) \
    QStringList & operator >> (QStringList &, TYPE &) throw (QString); \
    QStringList & operator << (QStringList &, const TYPE &)

    //! This macro provides string serialize operators.
#define _DJV_STRING_OPERATOR_LABEL(TYPE, LABEL) \
    QStringList & operator >> (QStringList & in, TYPE & out) \
        throw (QString) \
    { \
        if (! djv::Core::Serialize(in, out, LABEL)) \
        { \
            throw in.count() ? in[0] : QString(); \
        } \
        return in; \
    } \
    QStringList & operator << (QStringList & out, const TYPE & in) \
    { \
        return out << djv::Core::Serialize(in, LABEL); \
    }

} // namspace djv

#include <djvCore/StringUtilInline.h>
