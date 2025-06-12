#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ------------------ CONFIG ------------------
const char* ssid = "XOX_Game"; // AP mode
const char* password = "12345678";

ESP8266WebServer server(80);

// Button Pins
const uint8_t BTN_UP = 5;     // D1
const uint8_t BTN_LEFT = 4;   // D2
const uint8_t BTN_RIGHT = 14; // D5
const uint8_t BTN_SELECT = 12;// D6

// Game State
int cursorPos = 0;        // 0 to 8 (3x3 grid)
char board[9];            // ' ', 'X', 'O'
char currentPlayer = 'X';
char gameOver = ' ';      // 'X', 'O', 'D', or ' '
int scoreX = 0, scoreO = 0;

// Improved Button Debounce Structure
struct Button {
  uint8_t pin;
  bool lastState;
  bool currentState;
  unsigned long lastDebounceTime;
};

// Button instances
Button btnUp = {BTN_UP, HIGH, HIGH, 0};
Button btnLeft = {BTN_LEFT, HIGH, HIGH, 0};
Button btnRight = {BTN_RIGHT, HIGH, HIGH, 0};
Button btnSelect = {BTN_SELECT, HIGH, HIGH, 0};

const int debounceDelay = 50; // 50ms debounce time

void setupBoard() {
  for (int i = 0; i < 9; i++) board[i] = ' ';
  cursorPos = 0;
  currentPlayer = 'X';
  gameOver = ' ';
}

void switchPlayer() {
  currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

char checkWin() {
  const uint8_t wins[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8},  // rows
    {0,3,6}, {1,4,7}, {2,5,8},  // columns
    {0,4,8}, {2,4,6}            // diagonals
  };
  
  for (auto& w : wins) {
    if (board[w[0]] != ' ' && 
        board[w[0]] == board[w[1]] && 
        board[w[1]] == board[w[2]]) {
      return board[w[0]];
    }
  }
  return ' ';
}

bool isDraw() {
  for (int i = 0; i < 9; i++) {
    if (board[i] == ' ') return false;
  }
  return true;
}

bool debounce(Button &button) {
  bool reading = digitalRead(button.pin);
  
  if (reading != button.lastState) {
    button.lastDebounceTime = millis();
  }
  
  if ((millis() - button.lastDebounceTime) > debounceDelay) {
    if (reading != button.currentState) {
      button.currentState = reading;
      return true; // State changed
    }
  }
  
  button.lastState = reading;
  return false; // No state change
}

void handleInput() {
  if (gameOver != ' ') return;

  // Handle Up button
  if (debounce(btnUp)) {
    if (btnUp.currentState == LOW) {
      cursorPos = (cursorPos + 6) % 9; // Move up
    }
  }

  // Handle Left button
  if (debounce(btnLeft)) {
    if (btnLeft.currentState == LOW) {
      cursorPos = (cursorPos + 8) % 9; // Move left
    }
  }

  // Handle Right button
  if (debounce(btnRight)) {
    if (btnRight.currentState == LOW) {
      cursorPos = (cursorPos + 1) % 9; // Move right
    }
  }

  // Handle Select button
  if (debounce(btnSelect)) {
    if (btnSelect.currentState == LOW && board[cursorPos] == ' ') {
      board[cursorPos] = currentPlayer;
      char winner = checkWin();
      if (winner == 'X') { 
        scoreX++; 
        gameOver = 'X'; 
      } else if (winner == 'O') { 
        scoreO++; 
        gameOver = 'O'; 
      } else if (isDraw()) { 
        gameOver = 'D'; 
      } else {
        switchPlayer();
      }
    }
  }
}

String renderBoard() {
  String html = R"=====(
<html>
<head>
  <title>ESP8266 Tic-Tac-Toe</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }
    h1 { color: #333; }
    table { margin: 0 auto; border-collapse: collapse; }
    td { 
      width: 60px; 
      height: 60px; 
      border: 2px solid #333; 
      font-size: 36px; 
      font-weight: bold;
      text-align: center;
      vertical-align: middle;
    }
    .highlight { background-color: #ff0; }
    .controls { margin: 20px; }
    button { 
      padding: 10px 20px; 
      font-size: 16px; 
      margin: 0 10px; 
      background-color: #4CAF50;
      color: white;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }
    button:hover { background-color: #45a049; }
    .status { font-size: 24px; margin: 20px; }
  </style>
</head>
<body>
  <h1>Tic-Tac-Toe</h1>
  <div class="status">
    Player: )=====";
  html += String(currentPlayer);
  
  html += R"=====(
  </div>
  <table>
)=====";

  // Render the 3x3 board
  for (int row = 0; row < 3; row++) {
    html += "<tr>";
    for (int col = 0; col < 3; col++) {
      int pos = row * 3 + col;
      String cellValue = board[pos] == ' ' ? "&nbsp;" : String(board[pos]);
      String highlightClass = (pos == cursorPos) ? " class='highlight'" : "";
      html += "<td" + highlightClass + ">" + cellValue + "</td>";
    }
    html += "</tr>";
  }

  html += R"=====(
  </table>
  <div class="status">
    Score: X = )=====";
  html += String(scoreX);
  html += " | O = ";
  html += String(scoreO);
  html += R"=====(
  </div>
)=====";

  // Game over message
  if (gameOver == 'X' || gameOver == 'O') {
    html += "<div class='status' style='color:green;'>Player ";
    html += String(gameOver);
    html += " Wins!</div>";
  } else if (gameOver == 'D') {
    html += "<div class='status'>It's a Draw!</div>";
  }

  // Control buttons
  html += R"=====(
  <div class="controls">
    <form method='GET' action='/'>
      <button type='submit' name='act' value='next'>Next Game</button>
      <button type='submit' name='act' value='reset'>Reset Score</button>
    </form>
  </div>
  <script>
    // Auto-refresh every 1.5 seconds
    setTimeout(function() {
      location.reload();
    }, 1500);
  </script>
</body>
</html>
)=====";

  return html;
}

void handleRoot() {
  if (server.hasArg("act")) {
    String act = server.arg("act");
    if (act == "next") {
      setupBoard();
    } else if (act == "reset") {
      setupBoard();
      scoreX = 0;
      scoreO = 0;
    }
    server.sendHeader("Location", "/");
    server.send(303);
    return;
  }
  server.send(200, "text/html", renderBoard());
}

void setup() {
  Serial.begin(115200);
  delay(100);
  
  // Initialize game board
  setupBoard();
  
  // Set up buttons with internal pull-up resistors
  pinMode(btnUp.pin, INPUT_PULLUP);
  pinMode(btnLeft.pin, INPUT_PULLUP);
  pinMode(btnRight.pin, INPUT_PULLUP);
  pinMode(btnSelect.pin, INPUT_PULLUP);
  
  // Start WiFi Access Point
  WiFi.softAP(ssid, password);
  Serial.println("");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // Set up web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  handleInput();
}