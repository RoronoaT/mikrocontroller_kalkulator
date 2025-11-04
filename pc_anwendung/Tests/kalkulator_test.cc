#include <gtest/gtest.h>
#include <termios.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>

 
std::string berechnen(std::string n1, std::string n2, char zeichen)
{
    
    std::string benutzerberechnung_anfrage = n1 + zeichen + n2;

    //Anschluss, an den das Arduino angeschlossen ist
    const char* port_name = "/dev/cu.usbmodem14701"; 
    //serielle Schnittstelle im Lese-/Schreibmodus öffnen,
    int serial_port = open(port_name, O_RDWR | O_NOCTTY); 

    //ueberprüfen, ob beim Öffnen des Ports ein Fehler aufgetreten ist.
    if (serial_port < 0) {
        //std::cerr << "Fehler beim Öffnen der seriellen Schnittstelle: " << strerror(errno) << std::endl;
    }

    // Serielle Schnittstelleneinstellungen konfigurieren
    struct termios tty;
    if (tcgetattr(serial_port, &tty) != 0) {
        //std::cerr << "Fehler beim Abrufen der Eigenschaften der seriellen Schnittstelle: " << strerror(errno) << std::endl;
        close(serial_port);
    }

    // baud rate setzen
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // 8N1 mode (8 data bits, no parity, 1 stop bit)
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag |= CREAD | CLOCAL;

    // flow control und echo deaktivieren
    tty.c_lflag = 0;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag = 0; // Raw output

    // non-blocking mit einer Zeitüberschreitung von 1 Sekunde festlegen
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 10; 

    //Ueberprüfen, ob bei der Konfiguration der seriellen Schnittstelle ein Fehler aufgetreten ist. 
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        //std::cerr << "Fehler beim Konfigurieren der seriellen Schnittstelle: " << strerror(errno) << std::endl;
        close(serial_port);
    }

    //Alle vorhandenen Eingabepufferdaten löschen
    tcflush(serial_port, TCIFLUSH);

    // Daten senden
    const char* benutzeranfrage = benutzerberechnung_anfrage.c_str();

    //delay in microseconds
    usleep(2000000);
    //Versuch, auf den seriellen Port zu schreiben
    int geschriebene_bytes = write(serial_port, benutzeranfrage, strlen(benutzeranfrage));

    // Einen Fehler melden, wenn der Versuch fehlgeschlagen ist
    if (geschriebene_bytes < 0) {
        //std::cerr << "Fehler beim Schreiben auf den seriellen Port: " << strerror(errno) << std::endl;
        close(serial_port);
    }
    //Warten, bis alle zuvor an den seriellen Port geschriebenen Daten gesendet wurden..
    tcdrain(serial_port);
    //std::cout << "An den Mikrocontroller gesendeter Berechnung: " << benutzeranfrage << std::endl;

    // Buffer zum Lesen der Nachricht vorbereiten
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    usleep(3000000);

    //Nachricht lesen
    int anzahl_der_bytes = read(serial_port, buffer, sizeof(buffer) - 1); 
    //Fehlermeldung, falls der Lesevorgang nicht erfolgreich war
    if (anzahl_der_bytes < 0) {
        //std::cerr << "Fehler beim Lesen vom seriellen Port: " << strerror(errno) << std::endl;
        close(serial_port);
    }
    // Seriellen Port schließen
    close(serial_port); 

    //usleep(2000000);
    return buffer;    
}

// Test 1
TEST(Soma, NumerosPositivos) {
EXPECT_EQ(berechnen("150", "534", '+'),"684");
}
TEST(Soma, NumerosNegativos) {
EXPECT_EQ(berechnen("-150", "150", '+'),"0");
}

// Test 2
TEST(Subtracao, NumerosPositivos) {
EXPECT_EQ(berechnen("150", "534", '-'),"-384");
}
TEST(Subtracao, NumerosNegativos) {
EXPECT_EQ(berechnen("-150", "534", '-'),"-684");
}

// Test 3
TEST(Multiplicacao, NumerosPositivos) {
EXPECT_EQ(berechnen("15", "53", 'x'),"795");
}
TEST(Multiplicacao, NumerosNegativos) {
EXPECT_EQ(berechnen("-50", "34", 'x'),"-1700");
}

// Test 4
//Using only one decimal point (call to .substr()) due to floating point imprecision 
TEST(Divisao, NumerosPositivos) {
EXPECT_EQ(berechnen("150", "534", ':').substr(0, 3),"0.2");
}
TEST(Divisao, NumerosNegativos) {
EXPECT_EQ(berechnen("-150", "3", ':').substr(0, 5),"-50.0");
}

// Test 5
TEST(Overflow, EntradasGrandes) {
EXPECT_EQ(berechnen("1507356745624563567", "533567354256456723574", '+'),"ovf");
}
TEST(Overflow, ResultadoGrande) {
EXPECT_EQ(berechnen("9875433412344", "1345343123534", 'x'),"ovf");
}