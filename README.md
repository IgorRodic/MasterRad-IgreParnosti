Projekat koji implementira centralni algoritam prikazan u master tezi "Igre parnosti i njihovo rešavanje svođenjem na SAT". 
Matematički fakultet, školska 2019/20.

Tema projekta je implementacija algoritma koji svodi problem rešavanja igara parnosti na SAT problem.
Nakon što se generiše formula opisanim algoritmom, ona se svodi na KNF i smešta u fajl formula.cnf koji je zatim
moguće proslediti proizvoljnom SAT rešavaču radi provere zadovoljivosti.
Implementirana su dva algoritma za svođenje dobijene formule na KNF, osnovni algoritam i Cejtinova transformacija.
Projekat je moguće testirati na grafovima Jurdžinskog, čije je generisanje takođe implementirano, ili na grafovima
zadatim u txt fajlu zadatog formata.
