#include "InfernoWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "DiskUtility.h"

// --- Implementation of InfernoWindow ---

InfernoWindow::InfernoWindow(QWidget *parent) : QMainWindow(parent), diskUtility(new DiskUtility(this)) {
    setupUI();
    setWindowTitle("Inferno - Bootable USB Creator (Developed by Ahmed Nour Ahmed)");
    setFixedSize(600, 450); // Fixed size for a clean look
}

InfernoWindow::~InfernoWindow() {
    // Clean up if necessary
}

void InfernoWindow::setupUI() {
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
    driveComboBox->addItem("Select a USB Drive...");
    updateDriveList(); // Populate the list on startup
    // TODO: Populate with actual detected drives using DiskUtility
    driveLayout->addWidget(driveComboBox);
    mainLayout->addLayout(driveLayout);

    // 4. Advanced Options Toggle
    advancedOptionsCheckBox = new QCheckBox("Show Advanced Features (Inferno Exclusive)", this);
    mainLayout->addWidget(advancedOptionsCheckBox);

    // 5. Advanced Features Group (Hidden by default)
    QWidget *advancedGroup = new QWidget(this);
    QVBoxLayout *advancedLayout = new QVBoxLayout(advancedGroup);
    
    // Feature 1: Persistence
    persistenceCheckBox = new QCheckBox("Enable Persistence (Save files/settings)", advancedGroup);
    advancedLayout->addWidget(persistenceCheckBox);
    
    // Feature 2: Multi-boot
    multiBootCheckBox = new QCheckBox("Multi-Boot Mode (Add to existing drive)", advancedGroup);
    advancedLayout->addWidget(multiBootCheckBox);
    
    // Feature 3: Windows 11 Bypass
    win11BypassCheckBox = new QCheckBox("Bypass Windows 11 Requirements (TPM/RAM)", advancedGroup);
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

void InfernoWindow::selectDiskImage() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select Disk Image (ISO/IMG)"), QDir::homePath(), tr("Disk Images (*.iso *.img);;All Files (*)"));

    if (!fileName.isEmpty()) {
        isoPathLabel->setText(fileName);
        startButton->setEnabled(true); // Enable start button for demonstration
        statusLabel->setText(tr("Image selected: %1").arg(QFileInfo(fileName).fileName()));
    }
}

void InfernoWindow::selectTargetDrive() {
    // This slot would be connected to driveComboBox's signal (currentIndexChanged)
    // and would handle drive selection logic.
}

void InfernoWindow::startBurningProcess() {
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

void InfernoWindow::toggleAdvancedOptions(bool checked) {
    if (checked) {
        statusLabel->setText("Advanced Inferno features enabled.");
    } else {
        statusLabel->setText("Advanced Inferno features disabled.");
}

void InfernoWindow::updateDriveList() {
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

void InfernoWindow::handleProgressUpdate(int percentage, const QString &message) {
    progressBar->setValue(percentage);
    statusLabel->setText(message);
}

void InfernoWindow::handleWriteCompletion(bool success, const QString &errorMessage) {
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
}
