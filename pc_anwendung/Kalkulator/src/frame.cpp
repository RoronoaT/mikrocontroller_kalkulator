#include "frame.h"
#include <wx/wx.h>
#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <ctime>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/statline.h>
using namespace std;

// IDs für die Widgets definieren
enum IDs 
{
    BERRECHNEN = 2,
    EINGABE1 = 4,
    EINGABE2 = 5,
    ADDITION = 6,
    SUBTRAKTION = 7,
    MULTIPLIKATION = 8,
    DIVISION = 9,
    LOESCHEN = 10,
};

ViewPort::ViewPort(const wxString& title)
       : wxFrame(NULL, -1, title, wxPoint(-1, -1), wxSize(475, 370))
{
  //Hauptfenster erstellen
  wxPanel *panel = new wxPanel(this, -1);
  //vertical sizer erstellen
  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

  //horizontal sizer 0 erstellen
  wxBoxSizer *hbox0 = new wxBoxSizer(wxHORIZONTAL);
  //static text 0 erstellen
  st0 = new wxStaticText(panel, wxID_ANY, benutzerberechnung_anfrage);
// Schriftgröße auf 12 Punkt einstellen
  wxFont font = st0->GetFont();
   font.SetPointSize(18);
   st0->SetFont(font);
    st0->GetParent()->Layout();
  //static text 0 in horizontal sizer 0 einfügen
  hbox0->Add(st0, 0, wxALIGN_LEFT | wxALL);
  //horizontal sizer 0 im vertical sizer ausrichten
  vbox->Add(hbox0, 0, wxALIGN_LEFT | wxALL, 20);
  //borders hinzufügen
  vbox->Add(-1, 5);
  //horizontal sizer 01 erstellen
  wxBoxSizer *hbox01 = new wxBoxSizer(wxHORIZONTAL);
  //button 0 erstellen
  wxButton *btn0 = new wxButton(panel, LOESCHEN, wxT("Loeschen"));
  //button im horizontal sizer 01 einfügen
  hbox01->Add(btn0, 0);
  //horizontal sizer 1 im vertical sizer ausrichten
  vbox->Add(hbox01, 0, wxALIGN_CENTER_HORIZONTAL | wxRIGHT | wxLEFT, 10);
  //borders hinzufügen
  vbox->Add(-1, 5);
  

  //horizontal sizer 1 erstellen
  wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
  //static text 1 erstellen
  wxStaticText *st1 =  new wxStaticText(panel, wxID_ANY, 
      wxT("Geben Sie die erste Zahl ein:"));
  //static text 1 im horizontal sizer 1 einfügen
  hbox1->Add(st1, 0);
  //horizontal sizer 1 im vertical sizer ausrichten
  vbox->Add(hbox1, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM| wxTOP, 5);
  //borders hinzufügen
  vbox->Add(-1, 5);

  //horizontal sizer 2 erstellen
  wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
  //text inputl field erstellen - text control 1
  wxTextCtrl *tc1 = new wxTextCtrl(panel, EINGABE1);
  //text control 1 in in horizontal sizer 2 einfügen
  hbox2->Add(tc1, 0);
  //horizontal sizer 2 im vertical sizer ausrichten
  vbox->Add(hbox2, 0,wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxTOP, 5);
  //borders hinzufügen
  vbox->Add(-1, 5);

  //horizontal sizer 3 erstellen
  wxBoxSizer *hbox3 = new wxBoxSizer(wxHORIZONTAL);
  //static text 2 erstellen
  wxStaticText *st2 =  new wxStaticText(panel, wxID_ANY, 
      wxT("Geben Sie die zweite Zahl ein:"));
  //static text 2 im horizontal sizer 3 einfügen
  hbox3->Add(st2, 0);
  //horizontal sizer 3 in vertical sizer ausrichten
  vbox->Add(hbox3, 0,wxALIGN_CENTER_HORIZONTAL | wxBOTTOM| wxTOP, 5);
  //borders hinzufügen
  vbox->Add(-1, 5);

  //horizontal sizer 4 erstellen
  wxBoxSizer *hbox4 = new wxBoxSizer(wxHORIZONTAL);
  //text inputl field erstellen - text control 2 
  wxTextCtrl *tc2 = new wxTextCtrl(panel, EINGABE2);
  //text control 2 in im horizontal sizer 4 einfügen
  hbox4->Add(tc2, 0);
  //horizontal sizer 4 im vertical sizer ausrichten
  vbox->Add(hbox4, 0, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxTOP, 5);
  //borders hinzufügen
  vbox->Add(-1, 5);

  //horizontal sizer 5 erstellen
  wxBoxSizer *hbox5 = new wxBoxSizer(wxHORIZONTAL);
  //static text 3 erstellen
  wxStaticText *st3 =  new wxStaticText(panel, wxID_ANY, 
      wxT("Waehlen Sie die Operation aus:"));
  //static text 3 im horizontal sizer 5 einfügen
  hbox5->Add(st3, 0);
  //horizontal sizer 5 in vertical sizer ausrichten
  vbox->Add(hbox5, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM | wxTOP, 5);
  //borders hinzufügen
  vbox->Add(-1, 5);



  //horizontal sizer 6 erstellen
  wxBoxSizer *hbox6 = new wxBoxSizer(wxHORIZONTAL);
  //button 1 erstellen
  wxButton *btn1 = new wxButton(panel, ADDITION, wxT("+"));
  //button 1 im horizontal sizer 6 einfügen
  hbox6->Add(btn1, 0);
  //button 2 erstellen
  wxButton *btn2 = new wxButton(panel, SUBTRAKTION, wxT("-"));
  //button 2 im horizontal sizer 6 einfügen
  hbox6->Add(btn2, 0, wxLEFT | wxBOTTOM , 5);
  //button 3 erstellen
  wxButton *btn3 = new wxButton(panel, MULTIPLIKATION, wxT("x"));
  //button 3 in horizontal sizer 6 einfügen
  hbox6->Add(btn3, 0, wxLEFT | wxBOTTOM , 5);
  //button 4 erstellen
  wxButton *btn4 = new wxButton(panel, DIVISION, wxT(":"));
  //button 4 in horizontal sizer 6 einfügen
  hbox6->Add(btn4, 0, wxLEFT | wxBOTTOM , 5);
  //horizontal sizer 6 im vertical sizer ausrichten
  vbox->Add(hbox6, 0, wxALIGN_CENTER_HORIZONTAL | wxRIGHT | wxLEFT, 10);
  //borders hinzufügen
  vbox->Add(-1, 5);

  //horizontal sizer7 erstellen
  wxBoxSizer *hbox7 = new wxBoxSizer(wxHORIZONTAL);
  //button 7 erstellen
  wxButton *btn7 = new wxButton(panel, BERRECHNEN, wxT("Berechnen"));
  //button 7 im horizontal sizer 7 einfügen
  hbox7->Add(btn7, 0);
  //horizontal sizer 7 im vertical sizer ausrichten
  vbox->Add(hbox7, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM | wxLEFT| wxTOP | wxRIGHT, 10);


  //den vertikalen Sizer das gesamte Panel erweitern lassen
  panel->SetSizer(vbox);
  Centre();
  //status bar erstellen, um auf Eingabefehler hinzuweisen
  CreateStatusBar();
  SetStatusText("Klicken Sie auf „Berechnen“, um das Ergebnis anzuzeigen.");
  
}

void ViewPort::berechnen(wxCommandEvent& evt)
{
    std::cout << benutzerberechnung_anfrage <<std::endl;
    //Anschluss, an den das Arduino angeschlossen ist
    const char* port_name = "/dev/cu.usbmodem14701"; 
    //serielle Schnittstelle im Lese-/Schreibmodus öffnen,
    int serial_port = open(port_name, O_RDWR | O_NOCTTY); 

    //ueberprüfen, ob beim Öffnen des Ports ein Fehler aufgetreten ist.
    if (serial_port < 0) {
        std::cerr << "Fehler beim Öffnen der seriellen Schnittstelle: " << strerror(errno) << std::endl;
        wxString e1 = wxString::Format("Fehler beim Öffnen der seriellen Schnittstelle:  %s", strerror(errno));
        wxLogStatus(e1);
    }

    // Serielle Schnittstelleneinstellungen konfigurieren
    struct termios tty;
    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Fehler beim Abrufen der Eigenschaften der seriellen Schnittstelle: " << strerror(errno) << std::endl;
        wxString e2 = wxString::Format("Fehler beim Abrufen der Eigenschaften der seriellen Schnittstelle:  %s", strerror(errno));
        wxLogStatus(e2);
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
        std::cerr << "Fehler beim Konfigurieren der seriellen Schnittstelle: " << strerror(errno) << std::endl;
        wxString e3 = wxString::Format("Fehler beim Konfigurieren der seriellen Schnittstelle: %s", strerror(errno));
        wxLogStatus(e3);
        close(serial_port);
    }

    //Alle vorhandenen Eingabepufferdaten löschen
    tcflush(serial_port, TCIFLUSH);

    // Daten senden
    const char* benutzeranfrage = benutzerberechnung_anfrage.c_str();

    //Ueberprüfen Sie, ob der Benutzer eine gültige Eingabe gemacht hat. 
    if (eingabevalidierung(benutzereingabe_1,benutzereingabe_1))
    {
        //delay in microseconds
        usleep(2000000);
        //Versuch, auf den seriellen Port zu schreiben
        int geschriebene_bytes = write(serial_port, benutzeranfrage, strlen(benutzeranfrage));

        // Einen Fehler melden, wenn der Versuch fehlgeschlagen ist
        if (geschriebene_bytes < 0) {
            std::cerr << "Fehler beim Schreiben auf den seriellen Port: " << strerror(errno) << std::endl;
            wxString e4 = wxString::Format("Fehler beim Schreiben auf den seriellen Port: %s", strerror(errno));
            wxLogStatus(e4);
            close(serial_port);
        }
        //Warten, bis alle zuvor an den seriellen Port geschriebenen Daten gesendet wurden..
        tcdrain(serial_port);
        std::cout << "An den Mikrocontroller gesendeter Berechnung: " << benutzeranfrage << std::endl;
        wxString benutzeranfrage_s = wxString::Format("An den Mikrocontroller gesendeter Berechnung: %s", benutzeranfrage);
        //Log die gesendete Nachricht in der Status bar protokollieren
        wxLogStatus(benutzeranfrage_s);

        // Buffer zum Lesen der Nachricht vorbereiten
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));

        //Nachricht lesen
        int anzahl_der_bytes = read(serial_port, buffer, sizeof(buffer) - 1); 
        //Fehlermeldung, falls der Lesevorgang nicht erfolgreich war
        if (anzahl_der_bytes < 0) {
            std::cerr << "Fehler beim Lesen vom seriellen Port: " << strerror(errno) << std::endl;
            wxString e5 = wxString::Format("Fehler beim Lesen vom seriellen Port: %s", strerror(errno));
            wxLogStatus(e5);
            close(serial_port);
        }

        // die vom Mikrocontroller empfangene string verarbeiten .
        cout << "Antwort vom Mikrocontroller: " << buffer << std::endl;
        string antwort = benutzereingabe_1 + ' ' + arithmetische_operation + ' ' + benutzereingabe_2 + " = " + buffer + '\n'; 
        wxString antwort_vom_mikrocontroller = wxString::Format("Antwort vom Mikrocontroller: %s", antwort);
        benutzerberechnung_anfrage = buffer;

        // die verarbeitete string in der Benutzeroberfläche anzeigen.
        st0->SetLabel(wxString::Format(wxT("%s"), benutzerberechnung_anfrage ));

        //log das verarbeitete string in der status bar
        wxLogStatus(antwort_vom_mikrocontroller);
        
        //die Antwort des Mikrocontrollers in einer Textdatei Speichern
        string berechnungen_log = benutzereingabe_1 + arithmetische_operation + benutzereingabe_2 + '=' + benutzerberechnung_anfrage;
        wxFile file("berechnungen_verlauf.txt", wxFile::write_append);
            
        if (file.IsOpened())
        {
            file.Write(berechnungen_log);
            file.Write('\n');
            file.Close();
        }
        else
        {
            wxMessageBox("Datei konnte nicht geöffnet werden!", "Fehler", wxOK | wxICON_ERROR);
        }
    }
    else
    {
        //Den Benutzer über Fehler in den eingegebenen Daten informieren
        wxLogStatus("Eingabefehler: Die angegebenen Werte müssen Zahlen sein.");
        //Do not display the operation
        benutzerberechnung_anfrage = "";
    }
    
    

    // Seriellen Port schließen
    close(serial_port);
}

void ViewPort::dividieren(wxCommandEvent& evt)
{
    //Division als die vom Mikrocontroller auszuführende Operation Wählen
    arithmetische_operation = ':';
    benutzerberechnung_anfrage = benutzereingabe_1 + arithmetische_operation + benutzereingabe_2 + '\n';
    st0->SetLabel(wxString::Format(wxT("%s"), benutzerberechnung_anfrage));
}
void ViewPort::multiplizieren(wxCommandEvent& evt)
{
    //Multiplikation als die vom Mikrocontroller auszuführende Operation wählen
    arithmetische_operation = 'x';
    //to_display = a + ' ' + operation + ' ' + b + '_';
    benutzerberechnung_anfrage = benutzereingabe_1 + arithmetische_operation + benutzereingabe_2 + '\n';
    st0->SetLabel(wxString::Format(wxT("%s"), benutzerberechnung_anfrage));
}
void ViewPort::subtrahieren(wxCommandEvent& evt)
{
    //Subtraktion als die vom Mikrocontroller auszuführende Operation wählen
    arithmetische_operation = '-';
    benutzerberechnung_anfrage = benutzereingabe_1 + arithmetische_operation + benutzereingabe_2 + '\n';
    st0->SetLabel(wxString::Format(wxT("%s"), benutzerberechnung_anfrage));
}
void ViewPort::addieren(wxCommandEvent& evt)
{
    //Addition als die vom Mikrocontroller auszuführende Operation wählen
    arithmetische_operation = '+';
    benutzerberechnung_anfrage = benutzereingabe_1 + arithmetische_operation + benutzereingabe_2 + '\n';
    st0->SetLabel(wxString::Format(wxT("%s"), benutzerberechnung_anfrage));
}
void ViewPort::eingabeänderung_1(wxCommandEvent& evt)
{
    //Die vom Benutzer eingegebene Zahl abrufen - erste Zahl
    benutzereingabe_1 = evt.GetString();
    //Ueberpruefen, ob die Eingabe eine gültige Zahl ist.
    if (eingabevalidierung(benutzereingabe_1, benutzereingabe_2))
    {
        benutzerberechnung_anfrage = benutzereingabe_1 + arithmetische_operation + benutzereingabe_2 + '\n';
    }
    // Zeige nichts an, außer beide Eingaben wurden eingegeben.
    if((benutzereingabe_1 == "") || ( benutzereingabe_2 == "") || (arithmetische_operation == ' '))
    {
        benutzerberechnung_anfrage = "";
    }

    st0->SetLabel(wxString::Format(wxT("%s"), benutzerberechnung_anfrage ));
}

void ViewPort::eingabeänderung_2(wxCommandEvent& evt)
{
    //Die vom Benutzer eingegebene Zahl abrufen - zweite Zahl
    benutzereingabe_2 = evt.GetString();
    //Ueberpruefen, ob die Eingabe eine gültige Zahl ist.
    if (eingabevalidierung(benutzereingabe_1, benutzereingabe_2))
    {
        benutzerberechnung_anfrage = benutzereingabe_1 + arithmetische_operation + benutzereingabe_2 + '\n';
    }
    // Zeige nichts an, außer beide Eingaben wurden eingegeben.
    if((benutzereingabe_1 == "") || ( benutzereingabe_2 == "") || (arithmetische_operation == ' '))
    {
        benutzerberechnung_anfrage = "";
    }

    st0->SetLabel(wxString::Format(wxT("%s"), benutzerberechnung_anfrage ));
}

bool ViewPort::eingabevalidierung(string i1, string i2)
{
    string zahlen [2] = {i1,i2};

    for(int a = 0; a < 2; a++)
    {
        int zaehler = 0;
        int anzahl_dezimaltrennzeichen = 0;
        for (char c : zahlen[a])
        {
            //Ist das erste Zeichen gerade in der Schleife?
            bool ist_erster_index = (zaehler == 0);
            //Ist diese Iteration die Mitte des Strings? 
            bool mitte_der_zeichenfolge = (zaehler != 0) && (zaehler != zahlen[a].length() - 1);
            //Ist das Zeichen in diesem Iterationszyklus ein Komma oder ein Punkt?
            bool dezimaltrennzeichen = (c == ',') || (c == '.');
            
            //Hat die Zahl mehr als ein Dezimaltrennzeichen?
            if((c == ',') || (c == '.'))
            {
            anzahl_dezimaltrennzeichen++;
            }

            //Ist das erste Zeichen ein anderes Symbol als „-“ (Minuszeichen)?
            if(ist_erster_index && (c != '-') && !isdigit(c))
            {
                cout << "Ungültige Nummer: Nicht numerisches Zeichen ungleich ‚-‘ in Index 0 des angegebenen Strings "<< "\n";
                return false;
            }
            
            //Sind die Zeichen in der Mitte des Strings anders als Komma oder Punkt (Dezimalzeichen)?
            if(mitte_der_zeichenfolge && !isdigit(c) && !dezimaltrennzeichen)
            {
                std::cout<<"Ungültige Nummer: unerwartete Zeichen in der Mitte des Strings "<< "\n";
                return false;
            }

            zaehler++;
        }

        //Die Eingabe als ungültig melden, da die Eingabe mehr als ein Dezimaltrennzeichen enthält.
        if(anzahl_dezimaltrennzeichen > 1)
        {
            std::cout<<"Ungültig: Zu viele Kommas"<< "\n";
            return false;
        } 
    }
    return true;
}

void ViewPort::display_loeschen(wxCommandEvent& evt)
{
    //Die an den Mikrocontroller zu sendende Anfrage löschen.
    benutzerberechnung_anfrage = "";
    arithmetische_operation = ' ';
    benutzereingabe_1 = "";
    benutzereingabe_2 = "";
    st0->SetLabel(wxString::Format(wxT("%s"), benutzerberechnung_anfrage));
}

BEGIN_EVENT_TABLE(ViewPort, wxFrame)
    EVT_BUTTON(BERRECHNEN,  ViewPort::berechnen)
    EVT_BUTTON(DIVISION,  ViewPort::dividieren)
    EVT_BUTTON(MULTIPLIKATION,  ViewPort::multiplizieren)
    EVT_BUTTON(SUBTRAKTION,  ViewPort::subtrahieren)
    EVT_BUTTON(ADDITION,  ViewPort::addieren)
    EVT_BUTTON(LOESCHEN,  ViewPort::display_loeschen)
    EVT_TEXT(EINGABE1,  ViewPort::eingabeänderung_1)
    EVT_TEXT(EINGABE2,  ViewPort::eingabeänderung_2)
END_EVENT_TABLE()