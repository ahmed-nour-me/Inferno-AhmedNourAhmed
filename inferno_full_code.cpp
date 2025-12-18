//================================================================================
// INFERNO - The Ultimate Bootable USB Creator
// Developed by Ahmed Nour Ahmed from Qena
//
// NOTE: This file combines all C++ code (main, GUI, and Disk Utility interfaces)
// into a single, large file as requested by the user.
// In a real-world project, this code would be split into multiple files (.h and .cpp)
// for better organization and maintainability.
//
// This code uses the Qt framework for the GUI and simulates Windows-specific
// low-level disk operations. To compile, you need a C++20 compiler and Qt6.
//================================================================================

#include <QApplication>
#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QFileInfo>
#include <QMap>
#include <QVariant>

// Forward Declaration
class DiskUtility;

//================================================================================
// 1. DiskUtility Structures and Interface (Simulated Windows Low-Level Access)
//================================================================================

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
 * NOTE: The actual low-level WinAPI calls are simulated here.
 */
class DiskUtility : public QObject {
    Q_OBJECT

public:
    explicit DiskUtility(QObject *parent = nullptr) : QObject(parent) {}
    
    /**
     * @brief Enumerates all removable drives connected to the system.
     * @return QList<DriveInfo> A list of detected removable drives.
     */
    QList<DriveInfo> enumerateRemovableDrives() {
        // NOTE: In a real Windows application, this function would use WinAPI calls
        // like GetLogicalDrives, GetDriveType, and DeviceIoControl to get detailed info.
        // For this example, we return dummy data representing two USB drives.
        
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

    /**
     * @brief Starts the asynchronous process of writing an image to a drive.
     * 
     * @param imagePath Path to the ISO/IMG file.
     * @param drivePath Device path of the target drive (e.g., \\\\.\\PhysicalDriveX).
     * @param options Burning options (e.g., persistence, multi-boot).
     * @return bool True if the process started successfully, false otherwise.
     */
    bool startImageWrite(const QString &imagePath, const QString &drivePath, const QMap<QString, QVariant> &options) {
        // NOTE: In a real application, this would start a QThread or a separate process
        // to handle the low-level, time-consuming disk write operation using superior
        // asynchronous I/O for speed optimization.
        
        qDebug() << "Starting simulated image write:" << imagePath << "to" << drivePath;
        qDebug() << "Options:" << options;

        // Simulate the asynchronous writing process with a QTimer
        QTimer::singleShot(1000, [this]() {
            emit progressUpdated(25, "Formatting drive and preparing partitions (Inferno Custom Partitioning)...");
        });
        QTimer::singleShot(3000, [this]() {
            emit progressUpdated(75, "Writing image data (Superior Asynchronous I/O in progress)...");
        });
        QTimer::singleShot(5000, [this]() {
            emit progressUpdated(90, "Verifying image integrity (SHA-256/Digital Signatures)...");
        });
        QTimer::singleShot(6000, [this]() {
            emit progressUpdated(100, "Verification and finalization complete. Bootable USB Ready.");
            emit writeCompleted(true, "");
        });

        return true;
    }

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
};

//================================================================================
// 2. InfernoWindow (GUI Implementation using Qt)
//================================================================================

/**
 * @brief The main window class for the Inferno application.
 * 
 * Inherits from QMainWindow and sets up the main user interface components.
 */
class InfernoWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor for InfernoWindow.
     * @param parent The parent widget (default is nullptr).
     */
    explicit InfernoWindow(QWidget *parent = nullptr) : QMainWindow(parent), diskUtility(new DiskUtility(this)) {
        setupUI();
        setWindowTitle("Inferno - Bootable USB Creator (Developed by Ahmed Nour Ahmed)");
        setFixedSize(600, 450); // Fixed size for a clean look
        updateDriveList();
    }
    
    /**
     * @brief Destructor for InfernoWindow.
     */
    ~InfernoWindow() override {}

private slots:
    // Slots for UI interaction
    void selectDiskImage() {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Select Disk Image (ISO/IMG)"), QDir::homePath(), tr("Disk Images (*.iso *.img);;All Files (*)"));

        if (!fileName.isEmpty()) {
            isoPathLabel->setText(fileName);
            startButton->setEnabled(true); // Enable start button for demonstration
            statusLabel->setText(tr("Image selected: %1").arg(QFileInfo(fileName).fileName()));
        }
    }
    
    void startBurningProcess() {
        // Check if a drive is selected (index > 0 because index 0 is "Select a USB Drive...")
        if (driveComboBox->currentIndex() == 0) {
            QMessageBox::warning(this, "Inferno Error", "Please select a target USB drive.");
            return;
        }
        
        // Get selected drive path (assuming we store it as UserData in the QComboBox)
        QString drivePath = driveComboBox->currentData().toString();
        QString imagePath = isoPathLabel->text();
        
        if (imagePath == "No image selected.") {
            QMessageBox::warning(this, "Inferno Error", "Please select a disk image (ISO/IMG).");
            return;
        }
        
        // Gather options
        QMap<QString, QVariant> options;
        options["persistence"] = persistenceCheckBox->isChecked();
        options["multiBoot"] = multiBootCheckBox->isChecked();
        options["win11Bypass"] = win11BypassCheckBox->isChecked();
        
        // Confirmation dialog (Crucial step before wiping a drive)
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "CONFIRM DESTRUCTION",
            QString("WARNING: All data on drive %1 will be DESTROYED.\n\nAre you absolutely sure you want to proceed with writing '%2' to this drive?")
                .arg(driveComboBox->currentText())
                .arg(QFileInfo(imagePath).fileName()),
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            statusLabel->setText("Operation cancelled by user.");
            return;
        }

        // Start the process
        if (diskUtility->startImageWrite(imagePath, drivePath, options)) {
            startButton->setEnabled(false);
            statusLabel->setText("Burning process initiated...");
            progressBar->setValue(0);
        } else {
            QMessageBox::critical(this, "Inferno Error", "Failed to start the image writing process.");
        }
    }
    
    void toggleAdvancedOptions(bool checked) {
        if (checked) {
            statusLabel->setText("Advanced Inferno features enabled.");
        } else {
            statusLabel->setText("Advanced Inferno features disabled.");
        }
    }

    void updateDriveList() {
        driveComboBox->clear();
        driveComboBox->addItem("Select a USB Drive..."); // Index 0
        
        QList<DriveInfo> drives = diskUtility->enumerateRemovableDrives();
        
        if (drives.isEmpty()) {
            driveComboBox->addItem("No removable drives detected.");
            driveComboBox->setEnabled(false);
            return;
        }
        
        driveComboBox->setEnabled(true);
        for (const auto &drive : drives) {
            QString sizeStr = QString::number(drive.size / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
            QString itemText = QString("%1 (%2) - %3").arg(drive.driveLetter).arg(drive.model).arg(sizeStr);
            
            // Store the device path as UserData for easy retrieval during burning
            driveComboBox->addItem(itemText, drive.devicePath);
        }
    }

    void handleProgressUpdate(int percentage, const QString &message) {
        progressBar->setValue(percentage);
        statusLabel->setText(message);
    }

    void handleWriteCompletion(bool success, const QString &errorMessage) {
        startButton->setEnabled(true);
        progressBar->setValue(success ? 100 : progressBar->value());
        
        if (success) {
            statusLabel->setText("SUCCESS: Bootable USB created successfully!");
            QMessageBox::information(this, "Inferno Success", "The bootable USB drive has been created successfully!");
        } else {
            statusLabel->setText(QString("ERROR: %1").arg(errorMessage));
            QMessageBox::critical(this, "Inferno Error", QString("The process failed: %1").arg(errorMessage));
        }
    }

private:
    // UI Components
    QLabel *titleLabel;
    QComboBox *driveComboBox;
    QPushButton *selectIsoButton;
    QLabel *isoPathLabel;
    QCheckBox *advancedOptionsCheckBox;
    
    // Advanced Options Group
    QCheckBox *persistenceCheckBox;
    QCheckBox *multiBootCheckBox;
    QCheckBox *win11BypassCheckBox;
    
    QPushButton *startButton;
    QProgressBar *progressBar;
    QLabel *statusLabel;

    // Backend Utility
    DiskUtility *diskUtility;

    /**
     * @brief Sets up the main layout and components of the window.
     */
    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

        // 1. Title/Logo Area
        titleLabel = new QLabel("<h1>Inferno</h1>", this);
        titleLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(titleLabel);

        // 2. ISO Selection
        QHBoxLayout *isoLayout = new QHBoxLayout();
        selectIsoButton = new QPushButton("Select Disk Image (ISO/IMG)", this);
        isoPathLabel = new QLabel("No image selected.", this);
        isoLayout->addWidget(selectIsoButton);
        isoLayout->addWidget(isoPathLabel);
        mainLayout->addLayout(isoLayout);

        // 3. Drive Selection
        QHBoxLayout *driveLayout = new QHBoxLayout();
        driveLayout->addWidget(new QLabel("Target Drive:", this));
        driveComboBox = new QComboBox(this);
        driveLayout->addWidget(driveComboBox);
        mainLayout->addLayout(driveLayout);

        // 4. Advanced Options Toggle
        advancedOptionsCheckBox = new QCheckBox("Show Advanced Features (Inferno Exclusive)", this);
        mainLayout->addWidget(advancedOptionsCheckBox);

        // 5. Advanced Features Group (Hidden by default)
        QWidget *advancedGroup = new QWidget(this);
        QVBoxLayout *advancedLayout = new QVBoxLayout(advancedGroup);
        
        // Feature 1: Persistence (Full Support for major Linux distributions)
        persistenceCheckBox = new QCheckBox("Enable Full Persistence (Save files/settings on Live USB)", advancedGroup);
        advancedLayout->addWidget(persistenceCheckBox);
        
        // Feature 2: Multi-boot (Ventoy-like functionality)
        multiBootCheckBox = new QCheckBox("Multi-Boot Mode (Add ISO to existing drive without reformat)", advancedGroup);
        advancedLayout->addWidget(multiBootCheckBox);
        
        // Feature 3: Windows 11 Bypass (Integrated & Seamless)
        win11BypassCheckBox = new QCheckBox("Bypass Windows 11 Requirements (TPM/RAM/Secure Boot)", advancedGroup);
        advancedLayout->addWidget(win11BypassCheckBox);
        
        advancedGroup->setLayout(advancedLayout);
        advancedGroup->setVisible(false); // Initially hidden
        mainLayout->addWidget(advancedGroup);

        // 6. Start Button
        startButton = new QPushButton("START INFERNO PROCESS", this);
        startButton->setEnabled(false); // Disabled until ISO and Drive are selected
        mainLayout->addWidget(startButton);

        // 7. Progress and Status
        progressBar = new QProgressBar(this);
        progressBar->setTextVisible(true);
        progressBar->setValue(0);
        mainLayout->addWidget(progressBar);

        statusLabel = new QLabel("Ready.", this);
        mainLayout->addWidget(statusLabel);

        // --- Connections (Signals and Slots) ---
        connect(selectIsoButton, &QPushButton::clicked, this, &InfernoWindow::selectDiskImage);
        connect(startButton, &QPushButton::clicked, this, &InfernoWindow::startBurningProcess);
        connect(advancedOptionsCheckBox, &QCheckBox::toggled, advancedGroup, &QWidget::setVisible);
        connect(advancedOptionsCheckBox, &QCheckBox::toggled, this, &InfernoWindow::toggleAdvancedOptions);
        
        // Connect DiskUtility signals
        connect(diskUtility, &DiskUtility::progressUpdated, this, &InfernoWindow::handleProgressUpdate);
        connect(diskUtility, &DiskUtility::writeCompleted, this, &InfernoWindow::handleWriteCompletion);
    }
};

//================================================================================
// 3. Main Application Entry Point
//================================================================================

/**
 * @brief The main entry point for the Inferno application.
 * 
 * Initializes the Qt application, creates the main window, and starts the event loop.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments array.
 * @return int The application's exit code.
 */
int main(int argc, char *argv[]) {
    // Set application metadata
    QApplication::setApplicationName("Inferno");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("AhmedNourAhmed");
    
    // Initialize the Qt application
    QApplication app(argc, argv);

    // Create and show the main window
    InfernoWindow window;
    window.show();

    // Start the event loop
    return app.exec();
}

#include "inferno_full_code.moc"
