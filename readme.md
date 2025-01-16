# Mühle C++

Willkommen zum Mühle-Projekt!

## Inhalt

- [Voraussetzungen](#voraussetzungen)
- [Kompilieren und Ausführen](#kompilieren-und-ausführen)
- [Projektaufbau](#projektaufbau)
- [Spielablauf](#spielablauf)
- [Features](#features)
- [Beitrag leisten](#beitrag-leisten)

---

## Voraussetzungen

Um das Projekt erfolgreich auf macOS zu kompilieren und auszuführen, benötigst du folgende Tools:

1. **C++ Compiler**:
   - Installiere die Command Line Tools (falls nicht bereits vorhanden):
     ```bash
     xcode-select --install
     ```

2. **CMake**:
   - Installiere CMake über Homebrew:
     ```bash
     brew install cmake
     ```

3. **Terminal**:
   - Ein Standard-Terminal für macOS reicht aus, zur Entwicklung wurde iTerm2 verwendet.

---

## Kompilieren und Ausführen

### Schritte zum Kompilieren:

1. **Repository klonen:**
   ```bash
   git clone https://github.com/dein-github-benutzername/muehle.git
   cd muehle
   ```

2. **Build-Verzeichnis erstellen:**
   ```bash
   mkdir build
   cd build
   ```

3. **CMake-Projekt generieren:**
   ```bash
   cmake ..
   ```

4. **Projekt kompilieren:**
   ```bash
   make
   ```

5. **Spiel ausführen:**
   ```bash
   ./muehle
   ```

---

## Projektaufbau

Das Projekt ist modular aufgebaut, um die Lesbarkeit und Wartbarkeit zu verbessern:

```plaintext
.
├── BoardManager/
│   ├── BoardManager.cpp       # Verwaltung des Spielfelds und der Züge
│   ├── BoardManager.h
├── GameManager/
│   ├── GameManager.cpp        # Spielsteuerung und Phasenlogik
│   ├── GameManager.h
├── Logger/
│   ├── Logger.cpp             # Logger für Debugging und Fehlerberichte
│   ├── Logger.h
└── main.cpp                   # Einstiegspunkt des Programms
```

- **BoardManager:** Verwaltung der Spielfeldlogik (Setzen, Bewegen, Entfernen von Steinen).
- **GameManager:** Steuerung des Spielablaufs und der verschiedenen Phasen (Setzphase, Zugphase, Sprungphase).
- **Logger:** Ein Logging-System zur Protokollierung von Ereignissen und Fehlern.

---

## Spielablauf

Das Spiel folgt dem klassischen Ablauf von Mühle:

1. **Setzphase:**
   - Spieler setzen abwechselnd ihre Steine auf das Spielfeld.
   - Wenn drei Steine in einer Reihe (vertikal oder horizontal) stehen, bildet sich eine "Mühle" und der Spieler darf einen gegnerischen Stein entfernen.

2. **Zugphase:**
   - Spieler bewegen ihre Steine entlang der Nachbarfelder.
   - Mühlen können weiterhin gebildet werden, um gegnerische Steine zu entfernen.

3. **Sprungphase:**
   - Wenn ein Spieler nur noch 3 Steine hat, darf er auf beliebige freie Felder springen.

4. **Siegbedingungen:**
   - Ein Spieler gewinnt, wenn der Gegner:
     - Weniger als 3 Steine auf dem Spielfeld hat, oder
     - Keine legalen Züge mehr machen kann.

---

## Features

- **Pfeiltasten**: Bewegung über das Spielfeld
- **Leertaste (`Space`)**: Steine setzen
- **Enter (`Return`)**: Steine bewegen und anschließend mit `Space` setzen
- **Rücktaste (`Backspace`)**: Entfernen von gegnerischen Steinen

- **Speichern und Laden:** 
  - Das Spiel speichert den aktuellen Zustand in einer Datei `save.muehle`. 
  - Beim Start kann das Spiel fortgesetzt werden.

- **Benutzerdefinierte Symbole:**
  - Die Symbole der Spieler (z. B. "X" und "O") können im Einstellungsmenü geändert werden, auch Emojis werden unterstützt.

- **Farbliche Hervorhebungen:**
  - Aktuell ausgewählte Zellen werden farblich hervorgehoben.

- **Debugging-Logs:**
  - Alle wichtigen Aktionen werden im Logger dokumentiert (`game.log`).

