#include <EEPROM.h>
#include <avr/pgmspace.h>

const byte COL_9_PIN = 10; 
const byte SEG_CLK_PIN = 11;
const byte SEG_LATCH_PIN = 12;
const byte SEG_SER_PIN = 13;

#define MODE_ASSIST 0
#define MODE_PICK_BOX 1
#define MODE_PICK_CELL 2
#define MODE_PICK_DIGIT 3
#define MODE_TEXT 4
#define IS_PUZZLE_BIT   4 
#define IS_VISIBLE_BIT  5 
#define IS_WRONG_BIT    6 
#define IS_DOT_ON_BIT   7 
#define KEY_A   10
#define KEY_B   11
#define KEY_C   12
#define KEY_D   13
#define KEY_STAR  14
#define KEY_HASH  15

volatile byte gameMode = MODE_ASSIST;
volatile byte selectedBox = 0;
volatile byte selectedCell = 0;
volatile byte selectedDigit = 0;

const char sudokus[][81] PROGMEM = {
  "...1...3."
  "8.5.4..6."
  "32......5"
  "7....6.58"
  "........."
  ".6.51...."
  "2.......7"
  ".49.52..1"
  "..8.....4",

  "......8.." 
  ".987.43.."
  ".6.82.719"
  "....769.."
  "5...3.64."
  ".369..15."
  ".843..2.."
  "3..692.81"
  ".........",

  "..3.2.7.." 
  "6...35.91"
  ".17..98.."
  ".7..5..8."
  "..4.6.9.5"
  ".....71.2"
  "7.12.6.48"
  "..537...."
  "89251....",
};
volatile byte sudoku[9][9];

volatile int8_t gRow = 0;
volatile int8_t gCol = 0;
volatile byte refreshRate = 2; 
volatile byte refreshCounter = 0;
volatile byte antOffset = 0; 
volatile unsigned long animSpeed = 12; 
volatile unsigned long animCountdown = animSpeed;
volatile byte blinkSpeed = 12; 
volatile unsigned long blinkCountDown = blinkSpeed;
volatile bool blinkIsOn;
volatile byte dimSpeed = 48; 

volatile byte blinkBits[][2] = {
  {0, 1},
  {0, 0},
  {1, 0},
  {2, 0},
  {2, 1},
  {2, 2},
  {1, 2},
  {0, 2},
};
volatile byte animMax = sizeof(blinkBits) / (sizeof(byte) * 2);

int thresholds[16] = {0, 77, 144, 202, 244, 290, 331, 368, 394, 424, 452, 477, 496, 518, 538, 556};
char keypad[16] = {1, 2, 3, 10, 4, 5, 6, 11, 7, 8, 9, 12, 14, 0, 15, 13}; // A=10,B=11,C=12,D=13,*=14,#=15

volatile byte digitBits[] = {
  B00000000, 
  B00001100, 
  B11011010, 
  B10011110, 
  B00101110, 
  B10110110, 
  B11110110,
  B00011100, 
  B11111110, 
  B10111110, 
  B00000010, 
  B00111100, 
  B00000000, 
  B00000000, 
  B00000000, 
  B00000000, 
  B00000000, 

  B01111110, 
  B11111110, 
  B11110000, 
  B11001110, 
  B11110010, 
  B01110010, 
  B11110110, 
  B01101110, 
  B00001100, 
  B11001100, 
  B01101110, 
  B11100000, 
  B01111100, 
  B01000110, 
  B11111100, 
  B01111010, 
  B00111110, 
  B01111000, 
  B10110110, 
  B11100010, 
  B11101100, 
  B11101100, 
  B11101100, 
  B01101110,
  B10101110, 
  B11011010, 
  
  B00000000, 
  B00000000, 
  B00000000, 
  B00000000, 
  B10000000, 
  B00000000, 
  
  B11011110, 
  B11100110, 
  B11000010, 
  B11001110, 
  B11111010, 
  B01110010, 
  B10111110, 
  B01100110, 
  B01100000, 
  B11001100, 
  B01101110, 
  B01100000, 
  B01111100, 
  B01000110,
  B11000110, 
  B01111010, 
  B00111110, 
  B01111000,
  B10110110, 
  B11100010, 
  B11000100, 
  B11101100, 
  B11101100, 
  B01101110, 
  B10101110, 
  B11011010, 
};

void setup() {
  pinMode(SEG_CLK_PIN, OUTPUT);
  pinMode(SEG_LATCH_PIN, OUTPUT);
  pinMode(SEG_SER_PIN, OUTPUT);

  for (byte i = 0; i < 9; i++)
    pinMode(COL_9_PIN - i, OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(5));
  SetupTimer();
  gameMode = MODE_TEXT;
  ClearText();  
  //  DrawText(4,0,"CHECKER");
  //  delay(1000);
  //  DrawText(4,1, "HELLO");
  DrawText(3,3,   "The");
  DrawText(4,2,  "FinaL");
  DrawText(5,1, "Episode");
}


void loop() {WaitForKeypress();}

void PrintSudoku(){
  Serial.println();
  for (byte r = 0; r < 9; r++){
    for (byte c = 0; c < 9; c++) {
      if (sudoku[r][c] == 0) Serial.print("."); else Serial.print( sudoku[r][c], DEC);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void Swap(byte *a, byte *b){
  byte t = *a;
  *a = *b;
  *b = t;
}

void SudokuHFlip(){
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 4; c++) 
      Swap(&(sudoku[r][c]), &(sudoku[r][8 - c]));
}

void SudokuVFlip(){
  for (byte c = 0; c < 9; c++)
    for (byte r = 0; r < 4; r++) 
      Swap(&(sudoku[r][c]), &(sudoku[8 - r][c]));
}

void SudokuRotateClockwise(){
  for (byte i = 0; i < 4; i++) {
    for (byte j = i; j < (8 - i); j++) {
      byte t = sudoku[i][j]; 
      sudoku[i][j] = sudoku[8 - j][i]; 
      sudoku[8 - j][i] = sudoku[8 - i][8 - j]; 
      sudoku[8 - i][8 - j] = sudoku[j][8 - i]; 
      sudoku[j][8 - i] = t; 
    }
  }
}

void SudokuJumble(){
  byte translation[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (byte i = 0; i < 9; i++) {
    byte randomIndex = random(0, 9);
    Swap(&translation[i], &(translation[randomIndex]));
  }

  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
      if (sudoku[r][c] != 0) {
        byte originalDigit = sudoku[r][c];
        sudoku[r][c] = translation[originalDigit - 1];
      }
}

void SudokuGenerate(){
  noInterrupts(); 
  int puzzleMax = sizeof(sudokus) / 81;
  int randomPuzzle = random(0, puzzleMax);
  char puzzle[82];
  memcpy_P (puzzle, sudokus[randomPuzzle], 81);
  for (byte i = 0; i < 81; i++){
    byte r = i / 9;
    byte c = i % 9;
    sudoku[r][c] = puzzle[i] == '.' ? 0 : puzzle[i] - '0';
  }
  if (random(0, 2)) SudokuVFlip();
  if (random(0, 2)) SudokuHFlip();
  for (byte r = 0; r < random(0,4); r++)
    SudokuRotateClockwise();
  
  SudokuJumble();
  interrupts();
}

void SaveSudoku(){
  bool hasWrong = false;
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      if (bitRead(sudoku[r][c], IS_WRONG_BIT)) hasWrong = true;
      bitClear(sudoku[r][c], IS_DOT_ON_BIT); 
    }
  if (!hasWrong)
    EEPROM.put(0, sudoku);
}

void LoadSudoku(){EEPROM.get(0, sudoku);}

void SetupBoard(){
  SudokuGenerate();
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      if (sudoku[r][c] != 0) bitSet(sudoku[r][c], IS_PUZZLE_BIT); 
      bitSet(sudoku[r][c], IS_VISIBLE_BIT); 
    }
  SaveSudoku();
}

byte GetDigit(byte cellValue){ return cellValue & B00001111; }

void SetDot(byte row, byte col, byte state){
  if (state) bitSet(sudoku[row][col], IS_DOT_ON_BIT); else bitClear(sudoku[row][col], IS_DOT_ON_BIT);
}

void ClearSelection(){
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
      SetDot(r, c, 0); 
}

byte GetSelectedRow(byte selectedBox, byte selectedCell){
  byte boxRowOffset = (selectedBox / 3) * 3;
  byte cellRowOffset = (selectedCell / 3);
  return boxRowOffset + cellRowOffset;
}

byte GetSelectedCol(byte selectedBox, byte selectedCell){
  byte boxColOffset = (selectedBox % 3) * 3;
  byte cellColOffset = (selectedCell % 3);
  return boxColOffset + cellColOffset;
}

byte GetSelectedCell(byte selectedBox, byte selectedCell){
  byte row = GetSelectedRow(selectedBox, selectedCell);
  byte col = GetSelectedCol(selectedBox, selectedCell);
  byte cellValue = sudoku[row][col];
  return cellValue;
}

void SetSelectedCell(byte selectedBox, byte selectedCell, byte cellValue){
  byte row = GetSelectedRow(selectedBox, selectedCell);
  byte col = GetSelectedCol(selectedBox, selectedCell);
  sudoku[row][col] = cellValue;
}

void SetDigit(byte selectedBox, byte selectedCell, byte selectedDigit){
  byte cellValue = GetSelectedCell(selectedBox, selectedCell);
  if (!bitRead(cellValue, IS_PUZZLE_BIT)) {
    byte newCellValue = (cellValue & B11110000) | selectedDigit;
    SetSelectedCell(selectedBox, selectedCell, newCellValue);
  }
}

void RemoveWrong()
{
  byte r = GetSelectedRow(selectedBox, selectedCell);
  byte c = GetSelectedCol(selectedBox, selectedCell);
  if (bitRead(sudoku[r][c], IS_WRONG_BIT)) sudoku[r][c] = 0;
}

void ClearIsWrongs(){
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      bitClear(sudoku[r][c], IS_WRONG_BIT);
    }
}

void MarkAsWrong(byte row, byte col){bitSet(sudoku[row][col], IS_WRONG_BIT);}

void ValidateCell(byte row, byte col){
  ClearIsWrongs();

  bool hasWrong = false;
  byte guessDigit = GetDigit(sudoku[row][col]);

  for (byte c = 0; c < 9; c++){
    
    if ( c != col && GetDigit(sudoku[row][c]) == guessDigit) {
      MarkAsWrong(row, c);
      hasWrong = true;
    }
  }
  
  for (byte r = 0; r < 9; r++){
    if ( r != row && GetDigit(sudoku[r][col]) == guessDigit) {
      MarkAsWrong(r, col);
      hasWrong = true;
    }
  }
  byte r0 = (row / 3) * 3;
  byte c0 = (col / 3) * 3;
  for (byte r1 = 0; r1 < 3; r1++)
    for (byte c1 = 0; c1 < 3; c1++){
      byte r = r0 + r1;
      byte c = c0 + c1;
     
      if ( r != row && c != col && GetDigit(sudoku[r][c]) == guessDigit) {
        MarkAsWrong(r, c);
        hasWrong = true;
      }
    }
  if (hasWrong) MarkAsWrong(row, col);
}

void MarkRow(byte row, byte digit){
  for (byte c = 0; c < 9; c++)
    if (GetDigit(sudoku[row][c]) == 0) sudoku[row][c] = 10;
}

void MarkCol(byte col, byte digit){
  for (byte r = 0; r < 9; r++)
    if (GetDigit(sudoku[r][col]) == 0) sudoku[r][col] = 10;
}


void MarkBox(byte r0, byte c0, byte digit){
  for (byte r1 = 0; r1 < 3; r1++)
    for (byte c1 = 0; c1 < 3; c1++){
      byte r = r0 + r1;
      byte c = c0 + c1;
      if (GetDigit(sudoku[r][c]) == 0) sudoku[r][c] = 10;
    }
}

void ClearAssists(){
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++) {
      if (GetDigit(sudoku[r][c]) == 10 || bitRead(sudoku[r][c], IS_WRONG_BIT)) sudoku[r][c] = 0;
    }
}

void Clear(){ClearAssists();ClearSelection();}

void Assist(byte selectedDigit){
  ClearAssists();
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++){
      byte cellDigit = GetDigit(sudoku[r][c]);
      if (cellDigit == selectedDigit) {
        byte r0 = (r / 3) * 3;
        byte c0 = (c / 3) * 3;
        MarkRow(r, selectedDigit);
        MarkCol(c, selectedDigit);
        MarkBox(r0, c0, selectedDigit);
      }
    }

  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++){
      byte cellDigit = GetDigit(sudoku[r][c]);
      if (cellDigit == 0) {
        sudoku[r][c] = selectedDigit;
        MarkAsWrong(r, c);
      }
    }
}

void RedoGame(){
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++){
      byte cellDigit = GetDigit(sudoku[r][c]);
      if (bitRead(sudoku[r][c], IS_PUZZLE_BIT)){
        sudoku[r][c] = cellDigit;
        bitSet(sudoku[r][c], IS_PUZZLE_BIT);
        bitSet(sudoku[r][c], IS_VISIBLE_BIT);
      }
      else
        sudoku[r][c] = 0;
    }
  SaveSudoku();
}

void NewGame(){SetupBoard();}


void ClearText(){
  for (byte r = 0; r < 9; r++)
    for (byte c = 0; c < 9; c++)
      sudoku[r][c] = '0'; // 0 = blank space
}

void DrawText(byte row, byte col, String txt){
  for (byte c=0; c<txt.length(); c++){
    sudoku[row][col+c] = txt[c];
  }
}

void PleaseSelectBox_MarchingAnts(){
  if (--animCountdown == 0){
    for (byte i = 0; i < 9; i++){
      byte isOn = ! ((i - antOffset) % 3);
      SetDot(0, i, isOn); 
      SetDot(i, 8, isOn); 
      SetDot(8, 8 - i, isOn); 
      SetDot(8 - i, 0, isOn);
    }
    antOffset++;
    if (antOffset > 2) antOffset = 0;
    animCountdown = animSpeed;
  }
}

void PleaseSelectCell_MarchingAnts(){
  if (--animCountdown == 0){
    for (byte i = 0; i < animMax; i++){
      byte colOffset = (selectedBox % 3) * 3;
      byte rowOffset = (selectedBox / 3) * 3;
      byte c = colOffset + blinkBits[i][0];
      byte r = rowOffset + blinkBits[i][1];
      SetDot(r, c, !((i - antOffset) % 3)); 
    }
    antOffset++;
    if (antOffset > 2) antOffset = 0;
    animCountdown = animSpeed;
  }
}

void PleaseSelectDigit_Blink(){
  if (--animCountdown == 0){
    byte boxColOffset = (selectedBox % 3) * 3;
    byte boxRowOffset = (selectedBox / 3) * 3;
    byte cellColOffset = (selectedCell % 3);
    byte cellRowOffset = (selectedCell / 3);
    byte isOn = (antOffset > 0);
    SetDot(boxRowOffset + cellRowOffset, boxColOffset + cellColOffset, isOn);
    antOffset = isOn ? 0 : 1;
    animCountdown = animSpeed;
  }
}

void AnimateBlinks(){
  if (--blinkCountDown == 0){
    for (byte r = 0; r < 9; r++)
      for (byte c = 0; c < 9; c++) {
        if (!bitRead(sudoku[r][c],IS_WRONG_BIT))
          bitSet(sudoku[r][c], IS_VISIBLE_BIT);
        else
          if (blinkIsOn) 
          bitSet(sudoku[r][c], IS_VISIBLE_BIT); 
          else 
          bitClear(sudoku[r][c], IS_VISIBLE_BIT);
      }
    blinkIsOn = !blinkIsOn;
    blinkCountDown = blinkSpeed;
  }
}

void AssistDimming(){
  if (--blinkCountDown == 0){
    for (byte r = 0; r < 9; r++)
      for (byte c = 0; c < 9; c++) {
        byte cellDigit = GetDigit(sudoku[r][c]);
        if (cellDigit == selectedDigit || cellDigit == 10)
        if (blinkIsOn) 
          bitSet(sudoku[r][c], IS_VISIBLE_BIT); 
          else 
          bitClear(sudoku[r][c], IS_VISIBLE_BIT);
        else
          bitSet(sudoku[r][c], IS_VISIBLE_BIT);
      }
    blinkIsOn = !blinkIsOn;
    blinkCountDown = blinkIsOn ? 12 : dimSpeed;
  }
}

void AssistBlink(){
  if (--blinkCountDown == 0)
  {
    for (byte r = 0; r < 9; r++)
      for (byte c = 0; c < 9; c++) {
        byte cellDigit = GetDigit(sudoku[r][c]);
        if (cellDigit == selectedDigit || cellDigit == 10)
          if (blinkIsOn) 
          bitSet(sudoku[r][c], IS_VISIBLE_BIT); 
          else 
          bitClear(sudoku[r][c], IS_VISIBLE_BIT);
        else
          bitSet(sudoku[r][c], IS_VISIBLE_BIT);
      }
    blinkIsOn = !blinkIsOn;
    blinkCountDown = blinkIsOn ? 12 : dimSpeed;
  }
}

void HandleKeypress(char keyPress){
  switch (keyPress) {
    case KEY_A: 
      Clear();
      gameMode = MODE_PICK_BOX;
      break;
      
    case KEY_B: 
    case KEY_C: 
    case KEY_D: 
      Clear();
      SaveSudoku();
      gameMode = MODE_ASSIST;
      break;
      
    case KEY_STAR: 
      NewGame();
      gameMode = MODE_ASSIST;
      break;
      
    case KEY_HASH: 
      RedoGame();
      break;
      
    default:
      switch (gameMode) {
        case MODE_PICK_BOX:
          {
            ClearSelection();
            selectedBox = keyPress - 1;
            gameMode = MODE_PICK_CELL;
            break;
          }
          
        case MODE_PICK_CELL:
          {
            ClearSelection();
            selectedCell = keyPress - 1;
            byte cellValue = GetSelectedCell(selectedBox, selectedCell);
            if (!bitRead(cellValue, IS_PUZZLE_BIT)) gameMode = MODE_PICK_DIGIT;
            break;
          }
          
        case MODE_PICK_DIGIT:
          {
            selectedDigit = keyPress;
            SetDigit(selectedBox, selectedCell, selectedDigit);
            byte selectedRow = GetSelectedRow(selectedBox, selectedCell);
            byte selectedCol = GetSelectedCol(selectedBox, selectedCell);
            ValidateCell(selectedRow, selectedCol);
            SaveSudoku();
            break;
          }
        case MODE_ASSIST:
          {
            if (keyPress <= 9) {
              Assist(keyPress);
              selectedDigit = keyPress;
            }
            break;
          }
      }
      break;
  }
}

void WaitForKeypress(){
  int value = analogRead(A0);
  for (int i = 0; i < 16; i++){
    if ( abs(value - thresholds[i]) < 4){
      HandleKeypress(keypad[i]);
      while (analogRead(A0) < 1000) {
        delay(100);
      }
    }
  }
}

void FastSetColumn(byte col){
  col = 8 - col;
  byte bits = 0;
  if (col < 6) {
    bits = 1 << (col + 2); 
    PORTD = (PORTD & B00000011) | bits; 
    PORTB = PORTB & B11111000; 
  } 
  else{
    bits = 1 << (col - 6); 
    PORTD = PORTD & B00000011; 
    PORTB = (PORTB & B11111000) | bits;
  }
}

void DisplayCell(byte cellValue){
  byte cellDigit = GetDigit(cellValue);
  byte showDot = bitRead(cellValue, IS_DOT_ON_BIT);
  byte segByte =  bitRead(cellValue,IS_VISIBLE_BIT) ? digitBits[cellDigit] | showDot : digitBits[0];
  shiftOut(SEG_SER_PIN, SEG_CLK_PIN, LSBFIRST, ~segByte);
}

void DisplayText(byte cellValue){
  byte segByte =  digitBits[cellValue-'0'];
  shiftOut(SEG_SER_PIN, SEG_CLK_PIN, LSBFIRST, ~segByte);
}

void Refresh(void){
  if (--refreshCounter == 0) {
    refreshCounter = refreshRate;
    digitalWrite(A5, HIGH); // Debug
    for (gCol = 0; gCol < 9; gCol++){
      digitalWrite(SEG_LATCH_PIN, LOW); 
      for (gRow = 0; gRow < 9; gRow++){
        byte cellValue = sudoku[gRow][gCol];
        if (gameMode == MODE_TEXT)  DisplayText(cellValue);
        else                        DisplayCell( cellValue );
      }
      digitalWrite(SEG_LATCH_PIN, HIGH); 
      FastSetColumn(gCol);
    }
    digitalWrite(A5, LOW); 
  } 
  else{
    if (gameMode == MODE_PICK_BOX) PleaseSelectBox_MarchingAnts();
    if (gameMode == MODE_PICK_CELL) PleaseSelectCell_MarchingAnts();
    if (gameMode == MODE_PICK_DIGIT) PleaseSelectDigit_Blink();
    if (gameMode!=MODE_TEXT) AnimateBlinks(); 
  }
}

void SetupTimer(){
  OCR0A = 0xFF; 
  TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect){
  digitalWrite(A4, HIGH); 
  digitalWrite(A4, LOW);
  Refresh();
}

