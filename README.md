**Nume: SAMSON Alexandru-Valentin**
**Grupă: 312CC**


## Tema 1 – To pit or not to pit... this is the strategy


### Descriere:

* Acest cod oferă funcționalitate pentru analizarea și afișarea datelor de la
două tipuri de senzori: senzori de tip TIRE și senzori de tip PMU. Datele sunt
stocate în structuri, iar programul oferă funcții pentru afișarea detaliilor
senzorului și analiza datelor senzorului folosind un set de operații.

## Funcții

# void print(sensor *sensors, int index)

* Această funcție preia un vector de senzori și un index ca intrare și afișează
detaliile senzorului de la indexul dat. Detaliile afișate depind de tipul de
senzor. Pentru senzorii de tip TIRE, sunt afișate presiunea, temperatura,
nivelul de uzură și scorul de performanță (dacă este calculat). Pentru senzorii
PMU, sunt imprimate tensiunea, curentul, consumul de energie, regenerarea
energiei și stocarea energiei.

# void analyze(sensor *sensors, int index , void (*operations[])(void *))

* Această funcție preia un vector de senzori, un index și un vector de funcții
de operații ca intrare și efectuează setul de operații definite pentru senzor la
indexul dat. Operațiile sunt definite în fișierul operations.h și iau ca
intrare un pointer void.

# void clear (sensor **sensors, int *sensors_number)

* Această funcție preia un pointer către un vector de senzori și un pointer către
numărul de senzori ca intrare și efectuează validarea datelor pentru fiecare
senzor. Pentru senzorii de tip TIRE, funcția verifică dacă presiunea,
temperatura și nivelul de uzură se află în intervalele valide. Pentru senzorii
de tip PMU, funcția verifică dacă tensiunea, curentul, consumul de energie și
regenerarea energiei se află în intervalele valide. Dacă se constată că orice
date se află în afara intervalului valid, se consideră că senzorul este eronat
și este eliminat din vector.


### Comentarii asupra temei:

* Implementarea mi se pare destul de bună, am reușit să nu am leak-uri de
memorie, dar sigur era posibil să fie și mai eficientă sau mai scurtă. Totuși
sunt mulțumit cu această rezolvare.

* Tema această mi-a reamintit cum să lucrez cu cast-uri, vectori de pointeri la
funcții, fișiere header și fișiere binare.

### Punctajul obținut la teste local:

* TOTAL 100 / 100