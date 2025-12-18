#include "DiskUtility.h"
#include <QDebug>
#include <QTimer>

// --- Implementation of DiskUtility ---

DiskUtility::DiskUtility(QObject *parent) : QObject(parent) {
    // Constructor
}

QList<DriveInfo> DiskUtility::enumerateRemovableDrives() {
    // NOTE: In a real Windows application, this function would use WinAPI calls
    // like GetLogicalDrives, GetDriveType, and DeviceIoControl to get detailed info.
    // For this example, we return dummy data.
    
    QList<DriveInfo> drives;
    
    DriveInfo drive1;
    drive1.devicePath = "\\\\.\\PhysicalDrive1";
    drive1.driveLetter = "E:";
    drive1.model = "SanDisk Cruzer Blade (16 GB)";
    drive1.size = 16000000000LL; // 16 GB
    drive1.isRemovable = true;
    drives.append(drive1);

    DriveInfo drive2;
    drive2.devicePath = "\\\\.\\PhysicalDrive2";
    drive2.driveLetter = "F:";
    drive2.model = "Kingston DataTraveler (32 GB)";
    drive2.size = 32000000000LL; // 32 GB
    drive2.isRemovable = true;
    drives.append(drive2);

    qDebug() << "Simulated removable drives enumerated.";
    return drives;
}

bool DiskUtility::startImageWrite(const QString &imagePath, const QString &drivePath, const QMap<QString, QVariant> &options) {
    // NOTE: In a real application, this would start a QThread or a separate process
    // to handle the low-level, time-consuming disk write operation.
    
    qDebug() << "Starting simulated image write:" << imagePath << "to" << drivePath;
    qDebug() << "Options:" << options;

    // Simulate the asynchronous writing process with a QTimer
    QTimer::singleShot(1000, [this]() {
        emit progressUpdated(25, "Formatting drive and preparing partitions...");
    });
    QTimer::singleShot(3000, [this]() {
        emit progressUpdated(75, "Writing image data (Superior Asynchronous I/O in progress)...");
    });
    QTimer::singleShot(5000, [this]() {
        emit progressUpdated(100, "Verification and finalization complete.");
        emit writeCompleted(true, "");
    });

    return true;
}
