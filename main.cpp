#include <QApplication>
#include "src/InfernoWindow.h"

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
