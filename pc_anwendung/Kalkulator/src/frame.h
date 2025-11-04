#include <wx/wx.h>
using namespace std;

class ViewPort : public wxFrame
{
public:
  ViewPort(const wxString& title);
  void berechnen(wxCommandEvent& event);
  void addieren(wxCommandEvent& event);
  void subtrahieren(wxCommandEvent& event);
  void multiplizieren(wxCommandEvent& event);
  void dividieren(wxCommandEvent& event);
  void eingabeänderung_1(wxCommandEvent& event);
  void eingabeänderung_2(wxCommandEvent& event);
  void display_loeschen(wxCommandEvent& event);
  bool eingabevalidierung(string i1, string i2);
  std::string benutzereingabe_1;
  std::string benutzereingabe_2;
  std::string benutzerberechnung_anfrage;
  //static text
  wxStaticText *st0;
  char arithmetische_operation = ' ';
  
private:
    DECLARE_EVENT_TABLE()
};