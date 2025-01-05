# Roadmap für das Mühle-Brettspiel-Projekt (C++)

## Inhaltsverzeichnis
1. [Einleitung](#einleitung)
2. [Architektur und Komponenten](#architektur-und-komponenten)
3. [Roadmap](#roadmap)
4. [Komponenten-Details mit Funktionen und Methoden](#komponenten-details-mit-funktionen-und-methoden)

---

## Einleitung

Dieses Dokument beschreibt die Roadmap zur Umsetzung eines Mühle-Brettspiels in einer TUI-Anwendung (Text User Interface) mit C++. Die Struktur wurde so gestaltet, dass sie modular, effizient und wartbar ist. Wo sinnvoll, kommen C++-Bibliotheken wie Boost zum Einsatz.

---

## Architektur und Komponenten

Das Projekt wird in folgende Module unterteilt:
1. **Core Module (Spiel-Logik)**
    - Verwaltung des Spielfelds und der Spielregeln.
2. **TUI Module (Benutzeroberfläche)**
    - Darstellung des Start- und Pausemenüs sowie des Spielfelds.
3. **State Management (Status und Datenhaltung)**
    - Verwaltung des Spielstatus und der Spielstand-Speicherung.
4. **Services (Hintergrundlogik)**
    - Benutzerinteraktion und Dateioperationen.
5. **Utilities**
    - Logging und Fehlerbehandlung.

---

## Roadmap

### Phase 1: Grundgerüst und Spiel-Logik
1. **Spielfeld-Management**
    - Implementiere eine Datenstruktur zur Darstellung des Spielfelds (z. B. ein Array oder eine Graph-basierte Struktur).
    - Schreibe Methoden für das Platzieren, Bewegen und Entfernen von Steinen.

2. **Regeln und Bedingungen**
    - Implementiere die Regeln des Spiels, einschließlich Sieg- und Mühlenbildung.

---

### Phase 2: Benutzeroberfläche
1. **Hauptmenü**
    - Erstelle ein Startmenü mit Optionen wie "Neues Spiel", "Spiel laden" und "Beenden".

2. **Spielfeldanzeige**
    - Entwickle eine textbasierte Darstellung des Spielfelds.

3. **Pausemenü**
    - Füge ein Menü hinzu, das während des Spiels aufgerufen werden kann, mit Optionen wie "Fortsetzen", "Spiel speichern" und "Zum Hauptmenü".

---

### Phase 3: State Management
1. **Spielstand speichern und laden**
    - Verwende Boost.Serialization zur Serialisierung des Spielfelds und anderer Spielzustände.

2. **Statusverwaltung**
    - Implementiere eine Zustandsmaschine, um die verschiedenen Spielphasen zu verwalten.

---

### Phase 4: Feinschliff und Testing
1. **Fehlerbehandlung**
    - Implementiere robustes Fehlerhandling für Eingaben und Dateioperationen.

2. **Tests**
    - Schreibe Unit-Tests für die Spiellogik und Integrationstests für die Benutzeroberfläche.

---

## Komponenten-Details mit Funktionen und Methoden

### **Core Module**
#### 1. BoardManager (Spielfeld-Management)
**Wichtige Methoden:**
- `void initializeBoard()`: Initialisiert das Spielfeld.
- `bool setStone(int position, int player)`: Setzt einen Stein auf das Spielfeld.
- `bool moveStone(int from, int to, int player)`: Bewegt einen Stein.
- `bool removeStone(int position)`: Entfernt einen Stein.

#### 2. RuleEngine (Spielregeln)
**Wichtige Methoden:**
- `bool checkMill(int position)`: Prüft, ob eine Mühle gebildet wurde.
- `bool checkWin(int player)`: Überprüft, ob ein Spieler gewonnen hat.

---

### **TUI Module**
#### 1. MenuManager (Menü-Verwaltung)
**Wichtige Methoden:**
- `void showMainMenu()`: Zeigt das Hauptmenü an.
- `void showPauseMenu()`: Zeigt das Pausemenü an.

#### 2. GameView (Spielfeldanzeige)
**Wichtige Methoden:**
- `void renderBoard(const std::vector<int>& board)`: Zeigt das aktuelle Spielfeld an.
- `void displayPlayerTurn(int player)`: Zeigt den aktiven Spieler an.

---

### **State Management**
#### 1. GameStateManager
**Wichtige Methoden:**
- `void saveState(const std::string& filename)`: Speichert den aktuellen Spielstand.
- `void loadState(const std::string& filename)`: Lädt einen gespeicherten Spielstand.
- `void changeState(GameState newState)`: Ändert den aktuellen Spielstatus.

---

### **Services**
#### 1. InputHandler
**Wichtige Methoden:**
- `std::string getUserInput(const std::string& prompt)`: Holt Eingaben vom Benutzer.
- `int getPositionInput(const std::string& prompt)`: Holt eine gültige Spielfeldposition.

#### 2. FileService
**Wichtige Methoden:**
- `bool saveToFile(const std::string& data, const std::string& filepath)`: Speichert Daten in einer Datei.
- `std::string loadFromFile(const std::string& filepath)`: Lädt Daten aus einer Datei.

---

### **Utilities**
#### 1. Logger
**Wichtige Methoden:**
- `void logInfo(const std::string& message)`: Loggt allgemeine Informationen.
- `void logError(const std::string& message)`: Loggt Fehler.

---

## Fazit

Diese Roadmap und Komponentenübersicht bietet eine klare Struktur für die Umsetzung deines Mühle-Brettspiels. Die Boost-Bibliotheken (z. B. Boost.Serialization und Boost.Filesystem) helfen bei der effizienten Umsetzung der benötigten Funktionalitäten, während die modulare Architektur sicherstellt, dass das Projekt gut wartbar und erweiterbar bleibt.