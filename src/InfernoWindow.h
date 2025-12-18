#ifndef INFERNOWINDOW_H
#define INFERNOWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QCheckBox>

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
    explicit InfernoWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Destructor for InfernoWindow.
     */
    ~InfernoWindow() override;

private slots:
    // Slots for UI interaction
    void selectDiskImage();
    void selectTargetDrive();
    void startBurningProcess();
    void toggleAdvancedOptions(bool checked);
    void updateDriveList();
    void handleProgressUpdate(int percentage, const QString &message);
    void handleWriteCompletion(bool success, const QString &errorMessage);

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

    /**
     * @brief Sets up the main layout and components of the window.
     */
    void setupUI();
    
    // Backend Utility
    DiskUtility *diskUtility;
};

#endif // INFERNOWINDOW_H
