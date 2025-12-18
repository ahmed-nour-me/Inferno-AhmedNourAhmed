#ifndef DISKUTILITY_H
#define DISKUTILITY_H

#include <QString>
#include <QList>
#include <QObject>

/**
 * @brief Structure to hold information about a removable drive.
 */
struct DriveInfo {
    QString devicePath; // e.g., \\\\.\\PhysicalDrive1 or \\\\.\\E:
    QString driveLetter; // e.g., E:
    QString model;
    qint64 size; // Size in bytes
    bool isRemovable;
};

/**
 * @brief Utility class for low-level disk operations (Windows-specific).
 * 
 * This class handles drive enumeration, formatting, and raw data writing.
 */
class DiskUtility : public QObject {
    Q_OBJECT

public:
    explicit DiskUtility(QObject *parent = nullptr);
    
    /**
     * @brief Enumerates all removable drives connected to the system.
     * @return QList<DriveInfo> A list of detected removable drives.
     */
    QList<DriveInfo> enumerateRemovableDrives();

    /**
     * @brief Starts the asynchronous process of writing an image to a drive.
     * 
     * @param imagePath Path to the ISO/IMG file.
     * @param drivePath Device path of the target drive (e.g., \\\\.\\PhysicalDriveX).
     * @param options Burning options (e.g., persistence, multi-boot).
     * @return bool True if the process started successfully, false otherwise.
     */
    bool startImageWrite(const QString &imagePath, const QString &drivePath, const QMap<QString, QVariant> &options);

signals:
    /**
     * @brief Signal emitted to report the progress of the write operation.
     * @param percentage The current progress (0-100).
     * @param message A status message.
     */
    void progressUpdated(int percentage, const QString &message);

    /**
     * @brief Signal emitted when the write operation is complete.
     * @param success True if the operation succeeded, false otherwise.
     * @param errorMessage Error message if failure occurred.
     */
    void writeCompleted(bool success, const QString &errorMessage);

private:
    // Private implementation details for Windows API calls
    // ...
};

#endif // DISKUTILITY_H
