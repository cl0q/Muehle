* ## **Mühle C++**

  ### **Funktionale Anforderungen**

  **Spielfeld-Darstellung**

  - Das Spielfeld soll in der Konsole dargestellt werden.
  - Die Spielfelder sollen sichtbar und leicht verständlich sein (z. B. Buchstaben/Zahlen-Kombinationen für Positionen).

  **Spielmechanik**

  - Regeln von „Mühle“ werden korrekt umgesetzt.
  - Es soll möglich sein, Steine zu setzen, zu verschieben und Mühlen zu bilden.
  - Erkennung und Behandlung von Sieg-/Unentschieden-Bedingungen.

  **Spieler-Management**

  - Unterstützung für zwei Spieler.
  - Spieler können Namen eingeben.
  - Optionale Benutzericons (z. B. Buchstaben oder Symbole).

  **Interaktionslogik**

  - Spieler geben ihre Züge über die Tastatur ein (z. B. „A1-B2“).
  - Eingaben werden validiert (z. B. kein Zug außerhalb des Spielfelds oder doppelte Belegung).

  **Runden-Management**

  - Mehrere Runden können nacheinander gespielt werden.
  - Möglichkeit, das Spiel zu pausieren oder zu beenden.

  ### Zwischensapeicher

  

  ------

  ### **Nicht-funktionale Anforderungen**
  
  **Usability**
  
  - Intuitives Menüsystem mit klaren Optionen (z. B. „Neues Spiel“, „Anleitung“, „Beenden“).
  - Verständliche Fehlermeldungen bei ungültigen Eingaben.

  **Leistung**

  - Die Anwendung soll schnell starten und reaktionsschnell auf Eingaben reagieren.

  **Wartbarkeit**

  - Sauberer, modularer Code für leichte Erweiterbarkeit (z. B. Hinzufügen neuer Features).

  **Sicherheit**

  - Speicherlecks vermeiden durch Einsatz von modernen C++-Techniken (z. B. `std::unique_ptr`, `std::vector`).

  **Portabilität**

  - Die Anwendung soll auf verschiedenen Plattformen (Linux, Windows, macOS) laufen.
  
  **Optionale Features**
  
  - Einfache Animationen (z. B. für das Setzen von Steinen).
  - Anpassbare Spieler-Icons