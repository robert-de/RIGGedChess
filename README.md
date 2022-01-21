# Proiect Proiectarea Algoritmilor 2021 | 3 Check Chess
## Echipa 

 🐑 Cioban George-Adrian 

 🦝 Damian Robert-Eugen 

 🦊 Stanescu Gabriel 

 🦩 Strutu Ilinca-Ioana 

## Organizare

🍀 Instructiuni de compilare
    
    🌱 Trebuie sa existe folder-ul build.
    
    🌱 make build
    
    🌱 xboard -fcp "make run" -debug 

🍀 Clasele de piese

    🌱 Class Piece: 
        
        - Clasa de baza pentru piese si baza teblei de joc, care este o matrice 
          
          de 8x8 si este singleton. 
        
        - Fiecare instanta de Piece are un set de coordonate care se modifica la 
          
          fiecare mutare, culoarea piesei si functii care genereaza mutarile
          
          posibile in functie de situatia de pe tabla. 
    
    🌱 Class Pawn:
        
        - Pe langa generarea miscarilor de capturare si mutare, poate genera si
        
          miscarile de promovare si en passant.
    
    🌱 Class King:
        
        - Poate sa faca rocada daca se indeplinesc conditiile necesare (atat
        
          pentru rege, cat si pentru tura).
    
    🌱 Class Rook / Class Knight / Class Bishop / Class Queen :
        
        - Genereaza miscarile posibile pentru fiecare tip de piesa.
    
    🌱 Class chessMove:
        
        - Reprezinta o miscare de sah, cu pozitia sursa, pozitia destinatie si
        
          cu flag-uri pentru rocada, en passant si promovare care sunt setate pe
        
          true doar in cazul miscarii respective.
    
    🌱 Defenders si Contesters:
        
        - In cadrul fiecarei clase se adauga piesa in vectorul de defenders sau
        
          contesters pentru careurile pe care piesa poate sa ajunga, pe care le
        
          poate proteja sau pe care le poate ataca.

🍀 Mod de gandire
    
    🌱 Tabla este vizualizata ca o matrice de 8x8 de Cell-uri. Fiecare careu 
    
        contine piesa de pe sozitia respectiva (daca este cazul) si cei doi 
    
        vectori, defemders si contesters.
    
    🌱 Vectorul de defenders contine toate piesele care protejeaza careul 
    
        respectiv (piesele jucatorului).
    
    🌱 Vectorul de contesters contine toate piesele care pot ataca careul 
    
        respectiv (piesele adversarului).
    
    🌱 Dupa fiecare mutare a adversarului, se verifica daca suntem in sah. 
    
    🌱 In cazul in care suntem in sah, determinam numarul de atacatori. Daca e
    
        un singur atacator atunci adaugam la vectorul de mutari posibile toate 
        
        mutarile care ne scot din sah (care iau piesa care ataca regele, care 
        
        blocheaza calea piesei care ataca). Indiferent de numarul de atacatori, 
        
        adaugam miscarile regelui care ne scot din sah. Daca vectorul este gol,
        
        inseaman ca este sah mat si dam resign.
    
    🌱 In cazul in care nu suntem in sah, generam toate mutarile posibile si, 
        
        daca gasim o rocada o trimitem, altfel trimitem o mutare random din cele
        
        posibile.
    
    🌱 Daca luam de trei ori sah, dam resign si dam restart la tabla de joc.


