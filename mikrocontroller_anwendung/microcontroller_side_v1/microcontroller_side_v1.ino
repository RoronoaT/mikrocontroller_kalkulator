#include <stdlib.h>
#include <string.h>
#include <limits.h>

double reelle_zahl1 = 0;
double reelle_zahl2 = 0;
long natuerliche_zahl1= 0;
long natuerliche_zahl2 = 0;
bool add_overflow_natuerliche(long a, long b);
bool sub_overflow_natuerliche(long a, long b);
bool mul_overflow_natuerliche(long a, long b);
bool div_overflow_natuerliche(long a, long b);
bool ist_float_overflow(double a, double b);
bool is_overflow_on_konvertierung(String a, String b, char c);

void setup() {
	//Serielle Schnittstelle mit einer Baudrate von 115200 initialisieren
	Serial.begin(115200);
}

void loop() {
  //Serielle Schnittstelle lesen, wenn Daten erhalten werden
  if (Serial.available() > 0) 
  {
    String benutzer_anfrage = Serial.readStringUntil('\n');
    char arithmetische_operation;
    //Bestimmen, welche arithmetische Operation ausgeführt werden muss.
    int index = 0;
    bool operation_zeichen_gefunden = false;
    for(int a = 0; a < benutzer_anfrage.length(); a++)
    {
    	switch(benutzer_anfrage[a])
        {
            case '+':
              arithmetische_operation = '+';
                //Den Index des Pluszeichens in der Zeichenfolge bestimmen
                  index = a;
                  operation_zeichen_gefunden = true;
                break;
            case '-':
                arithmetische_operation = '-';
                //Den Index des Minuszeichens in der Zeichenfolge bestimmen
                if(a != 0) //Ignoriere das Minuszeichen, wenn es der erste Buchstabe in der Zeichenfolge ist (falls die erste Zahl negativ ist).
                {
                  index = a;
                  operation_zeichen_gefunden = true;
                }
                break;
            case ':':
                arithmetische_operation = ':';
                //Den Index des Divisionszeichens in der Zeichenfolge bestimmen
                  index = a;
                  operation_zeichen_gefunden = true;
                break;
            case 'x':
                arithmetische_operation = 'x';
                //Den Index des Multiplicationszeichens in der Zeichenfolge bestimmen
                  index = a;
                  operation_zeichen_gefunden = true;
                break;
            case ',':
                //Alle Kommas in den Zahlen durch einen Punkt ersetzen, da die Computeranwendung dem Benutzer die Möglichkeit bietet, entweder ein Komma oder einen Punkt als Dezimaltrennzeichen zu verwenden.
                benutzer_anfrage[a] = '.';
                break;
        }
        if (operation_zeichen_gefunden) break;
    }

    //Die Zahlen in der Zeichenfolge trennen (die vom Benutzer eingegebenen Zahlen 1 und 2).
    //Erste Benutzereingabe
    String benutzereingabe_1 = benutzer_anfrage.substring(0,index);
    //Zweite Benutzereingabe
    String benutzereingabe_2 = benutzer_anfrage.substring(index + 1, benutzer_anfrage.length());

    //Überprüfen, ob einer der Benutzereingaben eine reelle Zahl ist (das Dezimaltrennzeichen enthält).
    if (benutzer_anfrage.indexOf('.') != -1)
    {
        //Die Strings abschneiden, Leerstellen entfernen
    	  benutzereingabe_1.trim();
        benutzereingabe_2.trim();
        //Die Nullen im Dezimalteil der ersten Zahl entfernen 
        while (benutzereingabe_1.endsWith("0")) benutzereingabe_1.remove(benutzereingabe_1.length() - 1);
        if (benutzereingabe_1.endsWith(".")) benutzereingabe_1.remove(benutzereingabe_1.length() - 1);
        //Die Nullen im Dezimalteil der zweiten Zahl entfernen 
        while (benutzereingabe_2.endsWith("0")) benutzereingabe_2.remove(benutzereingabe_2.length() - 1);
        if (benutzereingabe_2.endsWith(".")) benutzereingabe_2.remove(benutzereingabe_2.length() - 1);
        //Die Strings abschneiden, Leerstellen entfernen
        benutzereingabe_1.trim();
        benutzereingabe_2.trim();

        //Überprüfen, ob die von der Computer-App gesendeten Zahlen bei der Konvertierung von String zu Float einen Float-Überlauf verursachen können (der Aufruf der Arduino-Funktion „.toDouble()“ gibt falsche Werte zurück)
        if(!is_overflow_on_konvertierung(benutzereingabe_1, benutzereingabe_2, 'f'))
        {
          //Die Zahlen konvertieren, wenn kein overflow aufgetreten ist.
        	reelle_zahl1 = benutzereingabe_1.toDouble();
          reelle_zahl2 = benutzereingabe_2.toDouble();
          /* for debugging purposes
          Serial.print("Real numbers were converted sucessfully: ");
          Serial.print(reelle_zahl1);
          Serial.print(reelle_zahl2);
          */
	      
	      //Die Benutzeranfrage (arithmetische Operation) ausführen.
        char arithmetische_operationen[4] = {'+', '-', 'x', ':'};
        for (int i = 0; i < sizeof(arithmetische_operationen); i++)
        {
          //check , ob das Ergebnis der arithmetische Operation den maximalen Speicherplatz überschreitet und einen Float-Überlauf verursacht.
          if(ist_float_overflow(reelle_zahl1, reelle_zahl2))
          {
            // Float-Überlaufausnahme auslösen – Benutzer über den Fehler warnen 
            Serial.print("ovf");  
          }
          else
          {
            // Arithmetische Operation ausführen
            // In den seriellen Port schreiben (das Ergebnis der arithmetischen Operation zurück an den Computer senden)
            if(i == 0 && arithmetische_operation == arithmetische_operationen[i]) 
            {
              Serial.print(reelle_zahl1 + reelle_zahl2);
            }
            else if(i == 1 && arithmetische_operation == arithmetische_operationen[i])
            { 
            Serial.print(reelle_zahl1 - reelle_zahl2);
            }
            if(i == 2 && arithmetische_operation == arithmetische_operationen[i])
            { 
              Serial.print(reelle_zahl1 * reelle_zahl2);
            }
            if(i == 3 && arithmetische_operation == arithmetische_operationen[i]) {
              Serial.print(reelle_zahl1 / reelle_zahl2);
            }
          }
        }
        }
        else
        {
          // Den Benutzer über eine fehlgeschlagene Konvertierung warnen – die angegebenen Zahlen überschreiten den Speicherplatz für Double-Werte.
        	Serial.print("ovf");
        }
      }
      else
      {
        // check, ob die von der PC-App gesendeten Zahlen bei der Konvertierung von String zu int einen int-Überlauf verursachen können (der Aufruf der Arduino-Funktion .toInt() gibt falsche Werte zurück)
        if(!is_overflow_on_konvertierung(benutzereingabe_1, benutzereingabe_2, 'i'))
        {
          //Die Zahlen konvertieren, wenn kein overflow aufgetreten ist.
        	natuerliche_zahl1= benutzereingabe_1.toInt();
        	natuerliche_zahl2 = benutzereingabe_2.toInt();
        	char arithmetische_operationen[4] = {'+', '-', 'x', ':'};
          for (int a = 0; a < sizeof(arithmetische_operationen); a++)
          {
            //check , ob das Ergebnis der arithmetische Operation den maximalen Speicherplatz überschreitet und einen integer-Überlauf verursacht.
            bool ovf = add_overflow_natuerliche(natuerliche_zahl1, natuerliche_zahl2) || sub_overflow_natuerliche(natuerliche_zahl1, natuerliche_zahl2) || mul_overflow_natuerliche(natuerliche_zahl1, natuerliche_zahl2) || div_overflow_natuerliche(natuerliche_zahl1, natuerliche_zahl2);
            if(ovf) 
            {
               // Integer-Überlaufausnahme auslösen – Benutzer über den Fehler warnen
              Serial.print("ovf"); 
              break;
            }
            else if(a == 0 && arithmetische_operation == arithmetische_operationen[a])
            {
              Serial.print(natuerliche_zahl1 + natuerliche_zahl2);
            }
            else if(a == 1   && arithmetische_operation == arithmetische_operationen[a])
            { 
              Serial.print(natuerliche_zahl1 - natuerliche_zahl2); 
            }
            else if(a == 2  && arithmetische_operation == arithmetische_operationen[a])
            { 
              Serial.print(natuerliche_zahl1 * natuerliche_zahl2); 
            }
            else if(a == 3  && arithmetische_operation == arithmetische_operationen[a]) 
            {
              Serial.print((float)natuerliche_zahl1 / (float)natuerliche_zahl2); 
            }        
          }
        }
        else
        {
          // Den Benutzer über eine fehlgeschlagene Konvertierung warnen – die angegebenen Zahlen überschreiten den Speicherplatz für Long-Werte.
          Serial.print("ovf");;
          return 1;
        }
      }
      delay(1000);
  }
}

bool add_overflow_natuerliche(long a, long b) 
{
  // check, ob die Summe der angegebenen Zahlen größer als die größtmögliche Ganzzahl (maximaler Speicherplatz für Ganzzahlen bei Arduino Uno) 
  // oder kleiner als die kleinstmögliche Ganzzahl (minimaler Speicherplatz für Ganzzahlen bei Arduino Uno) ist.
  return (b > 0 && a > LONG_MAX - b) || (b < 0 && a < LONG_MIN - b);
}

bool sub_overflow_natuerliche(long a, long b) 
{
  // check, ob die Subtraktion der angegebenen Zahlen größer als die größtmögliche Ganzzahl (maximaler Speicherplatz für Ganzzahlen bei Arduino Uno) 
  // oder kleiner als die kleinstmögliche Ganzzahl (minimaler Speicherplatz für Ganzzahlen bei Arduino Uno) ist.
  return (b < 0 && a > LONG_MAX + b) || (b > 0 && a < LONG_MIN + b);
}
/*
bool mul_overflow_natuerliche(long a, long b) 
{
  if (a == 0 || b == 0) return false;
  long ergebnis = a * b;
  //check, ob die Division zwischen dem Ergebnis und der zweiten Zahl die erste Zahl ergibt.
  return ergebnis / b != a;
}
*/
bool mul_overflow_natuerliche(long a, long b) 
{
  if (a == 0 || b == 0) return false;
  //überprüfen, ob das Ergebnis den Grenzwert überschreiten wird
  return a > (LONG_MAX / b);
}

bool div_overflow_natuerliche(long a, long b) 
{
  // check, ob der Divisor 0 ist (Unbestimmtheit)
  return b == 0 || (a == LONG_MIN && b == -1);
}

bool ist_float_overflow(double a, double b)
{
  // check, ob das Ergebnis der arithmetischen Operation unendlich groß, unendlich klein oder keine Zahl ist.
	double ergebnis = a * b;
	  if (isinf(ergebnis)) {
	    return true;
	  } else if (isnan(ergebnis)) {
	    return true;
	  } else {
	    return false;
	  }
}

bool is_overflow_on_konvertierung(String a, String b, char c)
{
    // um die konvertierten Ganzzahlen zu speichern
    long string_zu_natuerlicher_Zahl1 ;
    long string_zu_natuerlicher_Zahl2 ;
    // um die konvertierten Reellezahlen zu speichern
    double string_zu_reeller_Zahl1 ;
    double string_zu_reeller_Zahl2 ;
    // Konvertiere die oben genannte Ganzzahl oder Gleitkommazahl zurück in eine Zeichenfolge, um zu überprüfen, ob sie sich nach der Konvertierung geändert hat.
    String zurueck_zu_string1 ;
    String zurueck_zu_string2 ;
    // Hilfspuffer zum Speichern des Rückgabewerts der Funktion dtostrf() (präzise Konvertierung von Float in String)
    char reelle_zurueck_zu_string1[32];
    char reelle_zurueck_zu_string2[32];
    // Tests auf Konvertierungsfehler, wenn die Benutzereingaben Ganzzahlen sind
    if (c == 'i')
    {
      // Konvertiere die Eingabezeichenfolgen in Ganzzahlen. 
      string_zu_natuerlicher_Zahl1 = a.toInt();
      string_zu_natuerlicher_Zahl2 = b.toInt();
      // Das Ergebnis der Konvertierung wieder in eine Zeichenfolge umwandeln
      zurueck_zu_string1 = String(string_zu_natuerlicher_Zahl1);
      zurueck_zu_string2 = String(string_zu_natuerlicher_Zahl2);
      // check, ob die ursprünglich vom Benutzer angegebenen Zeichenfolgen mit den vom Arduino verarbeiteten Zeichenfolgen (der zurückkonvertierten Ganzzahl) übereinstimmen.
      if((a != zurueck_zu_string1) || (b != zurueck_zu_string2))
      {
        //Wenn die Zeichenfolgen unterschiedlich sind, ist bei der Konvertierung ein Fehler aufgetreten.
        return true;
      }
      // Wenn die Zeichenfolgen übereinstimmen, wurde die Zahl erfolgreich konvertiert.
      return false;
    }
    // Tests auf Konvertierungsfehler, wenn die Benutzereingaben Reelezahlen sind
    else if (c == 'f')
    {
      // Konvertiere die Eingabezeichenfolgen in Reelezahlen.
      string_zu_reeller_Zahl1 = a.toDouble();
      string_zu_reeller_Zahl2 = b.toDouble();
      
      // Konvertieren Sie die Fließkommazahlen zurück in Zeichenfolgen mit einer Genauigkeit von 6 Dezimalstellen und speichern Sie sie in separaten Puffern (Zeichenarray)
      dtostrf(string_zu_reeller_Zahl1, 0, 6, reelle_zurueck_zu_string1);
      dtostrf(string_zu_reeller_Zahl2, 0, 6, reelle_zurueck_zu_string2);
      
      // Die in den Puffern gespeicherten Werte in Variablen vom Typ String speichern
      zurueck_zu_string1 = String(reelle_zurueck_zu_string1);
      zurueck_zu_string2 = String(reelle_zurueck_zu_string2);
      
       // Entfernt bei Bedarf die Nullen rechts und das Dezimaltrennzeichen.
      
      //Die Strings abschneiden, Leerstellen entfernen
      zurueck_zu_string1.trim();
      zurueck_zu_string2.trim();
      
      //Die Nullen im Dezimalteil der ersten Zahl entfernen
      while (zurueck_zu_string1.endsWith("0")) zurueck_zu_string1.remove(zurueck_zu_string1.length() - 1);
      if (zurueck_zu_string1.endsWith(".")) zurueck_zu_string1.remove(zurueck_zu_string1.length() - 1);
      
      //Die Nullen im Dezimalteil der zweiten Zahl entfernen
      while (zurueck_zu_string2.endsWith("0")) zurueck_zu_string2.remove(zurueck_zu_string2.length() - 1);
      if (zurueck_zu_string2.endsWith(".")) zurueck_zu_string2.remove(zurueck_zu_string2.length() - 1);
      
      //Die Strings abschneiden, Leerstellen entfernen
      zurueck_zu_string1.trim();
      zurueck_zu_string2.trim();
      
      //Benutzer warnen, wenn die erste Eingabe eine leere Zeichenfolge war oder wenn die Konvertierung keine Zahl ergibt.
      bool float_overflow = false;
      if (!a.length() || isnan(string_zu_reeller_Zahl1)) {
        float_overflow = true;
      } else if (a.indexOf('.') != -1) {
        //avalia esta expressao boleana !!!
        //Fehlermeldungen für den Fall, dass die erste Eingabe eine von Null verschiedene reelle Zahl war und nach der Umwandlung Null wird. 
        //Verlust der Gleitkommagenauigkeit 
        if (string_zu_reeller_Zahl1 == 0.0 && a != "0" && a != "0.0") {
          float_overflow = true;
        }
      } else {
        // Ganze Zahl: Vergleicht die Eingabe und die Rückkonvertierung, um eine Trunkierung von Gleitkommazahlen zu erkennen.
        long lval1 = a.toInt();
        if (String(lval1) != a) {
          float_overflow = true;
        }
      }
      //Benutzer vor einem Fehler warnen, wenn die zweite Eingabe eine leere Zeichenfolge war oder wenn die Konvertierung keine Zahl ergibt.
      if (!b.length() || isnan(string_zu_reeller_Zahl2)) {
        float_overflow = true;
      } else if (b.indexOf('.') != -1) {
        //avalia esta expressao boleana !!!
        //Fehlermeldungen für den Fall, dass die erste Eingabe eine von Null verschiedene reelle Zahl war und nach der Umwandlung Null wird. 
        //Verlust der Gleitkommagenauigkeit 
        if (string_zu_reeller_Zahl2 == 0.0 && b != "0" && b != "0.0") {
          float_overflow = true;
        }
      } else {
        // Ganze Zahl: Vergleicht die Eingabe und die Rückkonvertierung, um eine Trunkierung von Gleitkommazahlen zu erkennen.
        long lval2 = b.toInt();
        if (String(lval2) != b) {
          float_overflow = true;
        }
      }
      
      //check ob die Funktion .toFloat() die Konvertierung fehlgeschlagen ist, check, ob sie den Dezimalteil der Zahl gelöscht hat und die Eingabe ursprünglich eine reelle Zahl war.
      if((zurueck_zu_string1.indexOf('.') == -1) && (a.indexOf('.') != -1))
      {
        //Wenn die konvertierte Zeichenfolge keinen Punkt enthält, deutet dies darauf hin, dass der Dezimalteil entfernt wurde, d. h., es ist ein Überlauf aufgetreten.
        float_overflow = true;
      }
      //check, ob die Funktion .toFloat() die Konvertierung fehlgeschlagen ist, check, ob sie den Dezimalteil der Zahl gelöscht hat und die Eingabe ursprünglich eine reelle Zahl war.
      if((zurueck_zu_string2.indexOf('.') == -1) && (b.indexOf('.') != -1))
      {
        //Wenn die konvertierte Zeichenfolge keinen Punkt enthält, deutet dies darauf hin, dass der Dezimalteil entfernt wurde, d. h., es ist ein Überlauf aufgetreten.
        float_overflow = true;
      }
      // for debuging purposes
      /*if (float_overflow)
      {
      	Serial.print("a: ");
      	//Serial.print(a);
      	//Serial.print(" string_zu_natuerlicher_Zahl1: ");
      	Serial.print(string_zu_reeller_Zahl2);
      	//Serial.print(" zurueck_zu_string1: ");
      	//Serial.print(zurueck_zu_string1);
      	delay(1000);	
      }*/
      
      
      return float_overflow;
    }
  
}