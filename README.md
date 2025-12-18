# Inferno - The Ultimate Bootable USB Creator

**Project Goal:** To create a powerful, fast, and feature-rich bootable USB creation tool for Windows, surpassing existing solutions like Rufus.

**Developer:** Ahmed Nour Ahmed (from Qena)

**Logo:** `inferno.png` (Placeholder for a future image asset)

## Key Features (Inferno vs. Rufus)

| Feature | Rufus | Inferno (Proposed) | Advantage |
| :--- | :--- | :--- | :--- |
| **Core Burning** | Yes | Yes | Standard functionality. |
| **Persistence** | Limited (e.g., Debian/Ubuntu) | **Full Support** for major Linux distributions (e.g., Ubuntu, Fedora, Kali) with dedicated partition creation. | Allows users to save files and settings on the Live USB. |
| **Multi-boot** | No | **Yes (Ventoy-like)** | Install multiple ISOs on a single drive without reformatting. |
| **ISO Downloader** | No (External script) | **Integrated** | Direct download of official ISOs (Windows, Linux) with automatic integrity checks. |
| **Integrity Check** | Limited (MD5/SHA1) | **Advanced (SHA-256/Digital Signatures)** | Ensures the downloaded/selected ISO is authentic and untampered. |
| **Speed Optimization** | Good | **Superior Asynchronous I/O** | Utilizes advanced Windows APIs for parallel and non-blocking disk writes, significantly reducing write time. |
| **Custom Partitioning** | No | **Advanced Editor** | Allows power users to define custom partition layouts (e.g., separate boot/data partitions). |
| **Windows 11 Bypass** | Yes (Registry hack) | **Integrated & Seamless** | One-click option to bypass TPM/Secure Boot/RAM checks during Windows 11 media creation. |
| **UI/UX** | Functional (Win32) | **Modern (Qt 6)** | Professional, customizable, and visually appealing interface. |

## Technology Stack

*   **Language:** C++20
*   **GUI Framework:** Qt 6 (for a modern, cross-platform-ready interface)
*   **Build System:** CMake
*   **Platform-Specific APIs (Windows):** WinAPI for low-level disk access (`CreateFile`, `DeviceIoControl`).
*   **Networking:** Qt Network Module (for ISO downloading and updates).
*   **Hashing:** OpenSSL or a built-in C++ library for cryptographic hashing.
