#ifndef PROTECTEDFILE_H
#define PROTECTEDFILE_H

#include "mssf-qt_global.h"

#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef MAEMO
namespace aegis {
class p_file;
}
#else
namespace mssf {
class p_file;
}
#endif

class MssfStorage;

class MSSFQTSHARED_EXPORT ProtectedFile
{
    friend class MssfStorage;

public:

    /*!
      * \brief Destructor
      */
    ~ProtectedFile();

    /*!
      * \brief Open file
      * \param flags \sa QFile::Permissions
      * \returns true if the file could be opened/created
      *
      * Opening of a file may fail because of the protection was violated.
      * For instance the file contents didn't match its stored hash (it has been tampered) or the
      * caller doesn't have the required credentials to open the file for writing or to decrypt and
      * encrypted file. In all these cases the errno will be set to EACCES.
      */
    bool open(QFile::Permissions flags);

    /*!
      * \brief Read data from a file
      * \param at The offset from which to read
      * \param len The number of bytes to read
      * \returns The read data, it may be less that len if EOF was encountered.
      */
    QByteArray read(quint64 at, quintptr len);

    /*!
      * \brief Write data to a file
      * \param at The offset to which to write
      * \param data The data to write
      * \returns The number of bytes actually written. Can be smaller than the given len if the
      *  filesystem is full.
      */
    qptrdiff write(quint64 at, QByteArray &data);

    /*!
      * \brief Truncate the file
      * \param at The new size of the file
      * \returns true on success, false otherwise. Use errno to find out the actual error condition.
      */
    bool trunc(quint64 at);

    /*!
      * \brief Close the file
      *
      * The contents of the file are flushed to the disk.
      */
    void close();

    /*!
      * \brief Is the file currently open
      * \returns true if it is, false otherwise
      *
      * Attempts to read or write into a non-open file return and error and errno is set to EBADF.
      */
    bool isOpen();

    /*!
      * \brief Get file status
      * \param st (in,out) A pointer to the status buffer
      * \returns true on success, false otherwise. Errno is set.
      * \todo Can this be replaced with a QFileInfo instead of stat ?
      */
    bool status(struct stat *st);

    /*!
      * \brief Get the digest of the file
      * \returns A base64 encoded hash computed of the current contents of the file,
      * terminated with a NULL.
      */
    QByteArray digest();

    /*!
      * \brief The name shown to outside
      * \returns The public name, not necessarily the place where the actual contents are stored.
      */
    QString name();

    /*!
      * \brief Return a pointer to the owning pstore
      * \returns A reference to the pstore this file belongs to
      */
    QSharedPointer<MssfStorage> owner();

    /*!
      * \brief Rename the file
      * \param newName The new name
      * \returns true on success, false otherwise. Errno is set.
      */
    bool rename(QString newName);

    /*!
      * \brief Change mode flags
      * \param flags new value of mode flags
      * \return true on success, false otherwise.
      */
    bool chmod(QFile::Permissions flags);

    /*!
      * \brief Change ownership
      * \param uid new file owner uid
      * \param gid new group owner gid
      * \return true on success, false otherwise
      */
    bool chown(uid_t uid, gid_t gid);

    /*!
      * \brief Change access times
      * \param accessTime The last time the file was accessed
      * \param modifiedTime The last time the file was modified.
      * \return true on success, false otherwise
      */
    bool utime(const QDateTime &accessTime, const QDateTime &modifiedTime);

private:

#ifdef MAEMO
    /*!
      * \brief Constructor private to allow only to reference a file within the storage area
      * \param file The file that is being accessed
      */
    ProtectedFile(aegis::p_file *file);
    //! The protected file that is wrapped
    aegis::p_file *file;
#else
    /*!
      * \brief Constructor private to allow only to reference a file within the storage area
      * \param file The file that is being accessed
      */
    ProtectedFile(mssf::p_file *file);
    //! The protected file that is wrapped
    mssf::p_file *file;
#endif
    //! A pointer to the owner of this protected file.
    QSharedPointer<MssfStorage> ownerPointer;
};

#endif // PROTECTEDFILE_H
