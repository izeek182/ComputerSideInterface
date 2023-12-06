// #include <fcntl.h>   // File Control Definitions
// #include <termios.h> // POSIX Terminal Control Definitions
// #include <unistd.h>  // UNIX Standard Definitions
// #include <errno.h>   // ERROR Number Definitions
// #include <stdio.h>

// int main() {
//     int fd; // File Descriptor

//     // Open the Port. We want read/write, no "controlling tty" behavior, and open it no matter what state DCD is in
//     fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);  

//     if(fd == -1) { // Error Checking
//        perror("Error - Unable to open UART. Ensure it is not in use by another application\n");
//     }

//     // Configure Port Settings
//     struct termios SerialPortSettings;
//     tcgetattr(fd, &SerialPortSettings); // Get the current attributes of the Serial port

//     cfsetispeed(&SerialPortSettings, B9600); // Set Read Speed
//     cfsetospeed(&SerialPortSettings, B9600); // Set Write Speed

//     SerialPortSettings.c_cflag &= ~PARENB;   // No Parity
//     SerialPortSettings.c_cflag &= ~CSTOPB;   // Stop bits = 1
//     SerialPortSettings.c_cflag &= ~CSIZE;    // Clears the Mask
//     SerialPortSettings.c_cflag |=  CS8;      // Set the data bits = 8

//     SerialPortSettings.c_cflag &= ~CRTSCTS;       // No Hardware flow Control
//     SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines

//     SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control
//     SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Non Cannonical mode

//     SerialPortSettings.c_oflag &= ~OPOST; // No Output Processing

//     // Set the attributes to the termios structure
//     if((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0) {
//         printf("\n  ERROR ! in Setting attributes\n");
//     } else {
//         printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none");
        
//         // Write data
//         char write_buffer[] = "Hello UART";  // Data to send
//         int  bytes_written  = write(fd, write_buffer, sizeof(write_buffer));

//         // Close the serial port
//         close(fd);
//     }

//     return 0;
// }


#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <glob.h>
#include <cstring>
#include <errno.h>

void checkSerialPorts() {
    glob_t glob_result;
    glob("/dev/ttyS*", GLOB_TILDE, NULL, &glob_result);  // This pattern may need adjustment based on the system

    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        const char* port = glob_result.gl_pathv[i];
        int fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);

        if (fd == -1) {
            if (errno == EBUSY) {
                std::cout << "Serial Port in use: " << port << std::endl;
            } else {
                std::cout << "Unable to open port " << port << ": " << strerror(errno) << std::endl;
            }
        } else {
            // Port is available
            close(fd);
        }
    }

    globfree(&glob_result);
}

int main() {
    std::cout << "Checking for open serial ports:" << std::endl;
    checkSerialPorts();
    return 0;
}
