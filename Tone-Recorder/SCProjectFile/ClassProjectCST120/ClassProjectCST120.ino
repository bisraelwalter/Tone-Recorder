#include <pitches.h>
#include <SD.h>
#include <LiquidCrystal.h>



// set up member variables
//****************************************************************************************************************************

Sd2Card card; // set up to check for SD card
LiquidCrystal lcd(9, 7, 5, 4, 3, 2); // initialize LCD display
File myFile; // use to open, modify, and close files

//****************************************************************************************************************************
//****************************************************************************************************************************


//set up global variables, consts, and array
//****************************************************************************************************************************

const int chipSelect = 10; //sets chipSelect for Uno board
const int menuPin = 15; // used for menu button
const int selectPin = 16;// used for select button
const int tonePin = 8;
int menuPinState = 0; // used to check and hold status of button press
int selectPinState = 0; // used to check and hold status of button press
int menuPressCounter = 0; // track number of menu presses
const int blueLED = 6; // set pin for the LED
char file[100]; //used to "record" tones and play

// Music Note Keys - Set frequency values
int notes[] = {262, 294, 330, 349, 392, 440, 494}; // Set notes C, D, E, F, G, A, B

//****************************************************************************************************************************
//****************************************************************************************************************************


// setup function
//****************************************************************************************************************************

void setup() {

  // start up lcd display
  lcd.begin(16, 2);
  lcd.display();
  lcd.clear(); // make sure display is empty

  // set up pins on microcontroller board
  pinMode(menuPin, INPUT);
  pinMode(selectPin, INPUT);
  pinMode(blueLED, OUTPUT); //are we keeping?
  digitalWrite(blueLED, LOW); // are we keeping?


  SD.begin(chipSelect); //start SD card

  //create files for recording
  myFile = SD.open("file1.txt", FILE_WRITE);
  myFile.close();

  myFile = SD.open("file2.txt", FILE_WRITE);
  myFile.close();

  myFile = SD.open("file3.txt", FILE_WRITE);
  myFile.close();

  myFile = SD.open("file4.txt", FILE_WRITE);
  myFile.close();

  myFile = SD.open("file5.txt", FILE_WRITE);
  myFile.close();


  Serial.begin(9600); // set up for debugging on serial monitor


  //running lcd to dispay initialization action
  digitalWrite(blueLED, LOW);
  lcd.print("   Initializing SD card...");
  for (int i = 0; i < 10; i++) {
    lcd.scrollDisplayLeft();
    delay(250);
  }

  cardCheck(); //check for sd card

}

//****************************************************************************************************************************
//****************************************************************************************************************************




//"main" function
//****************************************************************************************************************************
void loop() {

  menuPinState = digitalRead(menuPin);
  int keyVal = analogRead(A0);
  lcd.clear();
  lcd.print("Press <Menu> to");
  lcd.setCursor(0 , 1);
  lcd.print("   see files");
  delay(62);


  if (menuPinState == HIGH) {
    digitalWrite(blueLED, HIGH);
    fileScan();
  }

  ////// KEYBOARD NOTES & FREQUENCY STATEMENTS //////
  // Use the resistor ladder you created to change the voltage of each piano key button
  // 220 ohm = C5 // 560 ohm = D5 // 1K ohm = E5 // etc...
  if (keyVal >= 1011 & keyVal <= 1023)  {
    tone(tonePin, notes[0]); // C
  } // end if

  else if (keyVal >= 990 & keyVal <= 1010)  {
    tone(tonePin, notes[1]); // D
  } // end else if

  else if (keyVal >= 960 && keyVal <= 980)  {
    tone(tonePin, notes[2]); // E
  } // end else if

  else if (keyVal >= 900 && keyVal <= 945)  {
    tone(tonePin, notes[3]); // F
  } // end else if

  else if (keyVal >= 650 && keyVal <= 710)  {
    tone(tonePin, notes[4]); // G
  } // end else if

  else if (keyVal >= 500 && keyVal <= 550)  {
    tone(tonePin, notes[5]); // A
  } // end else if

  else if (keyVal >= 5 && keyVal <= 10)  {
    tone(tonePin, notes[6]); // B
  } // end else if

  else if (keyVal <= 1)  { // if no switch is pressed, do not play tone
    noTone(tonePin);
  } // end else if

} //end frequency tone loop

//****************************************************************************************************************************
//****************************************************************************************************************************


// program functions
//****************************************************************************************************************************


/*            *
******  FUNCTION  ******
 *            */
// function to check for SD card. If no card is found,
// prompts user to insert and reset device
void cardCheck()
{

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {

    //for LCD display
    lcd.clear();
    lcd.print(" Initialization");
    lcd.setCursor(0, 1);
    lcd.print("    Failure");
    delay(3000);


    while (menuPinState == 0) {
      menuPinState = digitalRead(menuPin);
      if (menuPinState == 1) digitalWrite(blueLED, HIGH);

      lcd.clear();
      lcd.print("Insert SD Card");
      lcd.setCursor(0, 1);
      lcd.print("and reset device");
      delay(50);
    }

    lcd.clear();
    menuPinState = 0;
    cardCheck();

  }

  else {
    lcd.clear();
    lcd.print("Setup success!!!");
    delay(3000);
  }
}


/*            *
******  FUNCTION  ******
 *            */
 //loops through files as menu is pressed.
 //File access depends on menuPressCounter's value
void fileScan()
{
  menuPinState = digitalRead(menuPin);
  selectPinState = digitalRead(selectPin);
  digitalWrite(blueLED, LOW);
  while (menuPressCounter < 5)
  {
    selectPinState = digitalRead(selectPin);
    menuPinState = 0;
    delay(100);
    while (menuPressCounter == 0)
    {
      menuPinState = digitalRead(menuPin);
      selectPinState = digitalRead(selectPin);
      lcd.clear();
      lcd.print("File 1");
      if (checkIfEmpty("file1.txt"))
      {
        emptyFileHandle("file1.txt");
      }
      else
      {
        fileWithTones("file1.txt");
      }

      if (menuPinState == HIGH) menuPressCounter++;
      menuPinState = 0;
      delay(100);

    }

    while (menuPressCounter == 1)
    {
      menuPinState = digitalRead(menuPin);
      selectPinState = digitalRead(selectPin);
      lcd.clear();
      lcd.print("File 2");
      if (checkIfEmpty("file2.txt"))
      {
        emptyFileHandle("file2.txt");
      }
      else
      {
        fileWithTones("file2.txt");
      }

      if (menuPinState == HIGH) menuPressCounter++;
      menuPinState = 0;
      delay(100);

    }

    while (menuPressCounter == 2)
    {
      menuPinState = digitalRead(menuPin);
      selectPinState = digitalRead(selectPin);
      lcd.clear();
      lcd.print("File 3");
      if (checkIfEmpty("file3.txt"))
      {
        emptyFileHandle("file3.txt");
      }
      else
      {
        fileWithTones("file3.txt");
      }

      if (menuPinState == HIGH) menuPressCounter++;
      menuPinState = 0;
      delay(100);
    }

    while (menuPressCounter == 3)
    {
      menuPinState = digitalRead(menuPin);
      selectPinState = digitalRead(selectPin);
      lcd.clear();
      lcd.print("File 4");
      if (checkIfEmpty("file4.txt"))
      {
        emptyFileHandle("file4.txt");
      }
      else
      {
        fileWithTones("file4.txt");
      }

      if (menuPinState == HIGH) menuPressCounter++;
      menuPinState = 0;
      delay(100);
    }

    while (menuPressCounter == 4)
    {
      menuPinState = digitalRead(menuPin);
      selectPinState = digitalRead(selectPin);
      lcd.clear();
      lcd.print("File 5");
      if (checkIfEmpty("file5.txt"))
      {
        emptyFileHandle("file5.txt");
      }
      else
      {
        fileWithTones("file5.txt");
      }

      if (menuPinState == HIGH) menuPressCounter++;
      menuPinState = 0;
      delay(100);
    }
  }
  menuPressCounter = 0;
}



/*            *
******  FUNCTION  ******
 *            */
// play file that was loaded from sd card
void playFile()
{
  lcd.clear();
  lcd.print("Playing File");
  for (int l = 0; l < 100; l++)
  {
    delay(62);
    if (file[l] == 48)
      noTone(tonePin);

    else if (file[l] == 49)
      tone(tonePin, notes[6]);

    else if (file[l] == 50)
      tone(tonePin, notes[5]);

    else if (file[l] == 51)
      tone(tonePin, notes[4]);

    else if (file[l] == 52)
      tone(tonePin, notes[3]);

    else if (file[l] == 53)
      tone(tonePin, notes[2]);

    else if (file[l] == 54)
      tone(tonePin, notes[1]);

    else if (file[l] == 55)
      tone(tonePin, notes[0]);
  }
  noTone(tonePin);
}



/*            *
******  FUNCTION  ******
 *            */
// record results to file array
void record()
{
  digitalWrite(blueLED, HIGH); // visual cue that recording has started

  for (int i = 0; i < 100; i++) { // continue loop while menu button isn't pressed
    int keyVal = analogRead(A0);
    lcd.clear();
    lcd.print("Recording");
    delay(62);

    ////// KEYBOARD NOTES & FREQUENCY STATEMENTS //////
    // Use the resistor ladder you created to change the voltage of each piano key button
    // 220 ohm = C5 // 560 ohm = D5 // 1K ohm = E5 // etc...
    if (keyVal >= 1011 & keyVal <= 1023)  {
      tone(tonePin, notes[0]); // C
      file[i] = 55;
    } // end if

    else if (keyVal >= 990 & keyVal <= 1010)  {
      tone(tonePin, notes[1]); // D
      file[i] = 54;
    } // end else if

    else if (keyVal >= 960 && keyVal <= 980)  {
      tone(tonePin, notes[2]); // E
      file[i] = 53;
    } // end else if

    else if (keyVal >= 900 && keyVal <= 945)  {
      tone(tonePin, notes[3]); // F
      file[i] = 52;
    } // end else if

    else if (keyVal >= 650 && keyVal <= 710)  {
      tone(tonePin, notes[4]); // G
      file[i] = 51;
    } // end else if

    else if (keyVal >= 500 && keyVal <= 550)  {
      tone(tonePin, notes[5]); // A
      file[i] = 50;
    } // end else if

    else if (keyVal >= 5 && keyVal <= 10)  {
      tone(tonePin, notes[6]); // B
      file[i] = 49;
    } // end else if

    else if (keyVal <= 1)  { // if no switch is pressed, do not play tone
      noTone(tonePin);
      file[i] = 48;
    } // end else if

  } //end frequency tone loop
  digitalWrite(blueLED, LOW); // turn off visual cue that recording is ended
}


/*            *
******  FUNCTION  ******
 *            */
 //loads file to array from SD card
void loadFile(char* theFile)
{
  myFile = SD.open(theFile, FILE_READ);
  while (myFile.available())
  {
    myFile.readBytesUntil('\0', file, 100);
  }

  myFile.close();
}


/*            *
******  FUNCTION  ******
 *            */
 //saves file from array to SD card
void saveFile(char* theFile)
{
  myFile = SD.open(theFile, FILE_WRITE);
  myFile.write(file, 100);
  myFile.close();
}



/*            *
******  FUNCTION  ******
 *            */
 //BOOLEAN - checks to see if file is empty
int checkIfEmpty(char* theFile)
{
  int value = 0;
  myFile = SD.open(theFile);
  if (myFile.size() == 0)
    value = 1;
  myFile.close();
  return value;

}

/*            *
******  FUNCTION  ******
 *            */
 //if file is empty, lets user know the file is empty
 //and prompts them the option to record
void emptyFileHandle(char* theFile)
{

  while (menuPinState == LOW)
  {
    menuPinState = digitalRead(menuPin);
    selectPinState = digitalRead(selectPin);
    lcd.setCursor(0, 1);
    lcd.print("(empty)  Record?");
    delay(50);

    if (selectPinState == HIGH)
    {
      record();
      saveFile(theFile);
      break;
    }
  }
}

/*            *
******  FUNCTION  ******
 *            */
 //if file has information, lets user know that 
 //there is a file and if they want to play it
void fileWithTones(char* theFile)
{
  menuPinState = digitalRead(menuPin);
  while (menuPinState == LOW)
  {
    menuPinState = digitalRead(menuPin);
    selectPinState = digitalRead(selectPin);
    lcd.setCursor(0, 1);
    lcd.print("Play file?");
    delay(50);

    if (selectPinState == HIGH)
    {
      loadFile(theFile);
      playFile();
    }
  }
  delay(100);
  fileOverwrite(theFile);

}

/*            *
******  FUNCTION  ******
 *            */
 //asks user if they would like to overwrite the information
 //that is in the file by recording something new
void fileOverwrite(char* theFile)
{
  menuPinState = digitalRead(menuPin);
  while (menuPinState == LOW)
  {
    menuPinState = digitalRead(menuPin);
    selectPinState = digitalRead(selectPin);
    lcd.setCursor(0, 1);
    lcd.print("Overwrite?");
    delay(50);

    if (selectPinState == HIGH)
    {
      record();
      saveFile(theFile);
      break;
    }
  }
  if (selectPinState == LOW) {
    delay(100);
    eraseFile(theFile);
  }
}

/*            *
******  FUNCTION  ******
 *            */
 //asks user if they would like to erase the information
 //that is in the file. 
void eraseFile(char* theFile)
{
  menuPinState = digitalRead(menuPin);
  while (menuPinState == LOW)
  {
    menuPinState = digitalRead(menuPin);
    selectPinState = digitalRead(selectPin);
    lcd.setCursor(0, 1);
    lcd.print("Erase file?");
    delay(50);

    if (selectPinState == HIGH)
    {
      SD.remove(theFile);
      myFile = SD.open(theFile, FILE_WRITE);
      myFile.close();
      break;
    }
  }

}


//****************************************************************************************************************************
//****************************************************************************************************************************

