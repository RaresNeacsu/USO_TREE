# Implementare Tree în C

Acest proiect este o implementare simplificată a comenzii `tree` în limbajul C. Programul afișează recursiv structura de fișiere și directoare pornind de la un director specificat, folosind caractere grafice pentru a reprezenta ierarhia.

## Funcționalități

- Listează conținutul directoarelor recursiv.
- Sortează intrările alfabetic.
- Afișează structura arborescentă folosind caractere ASCII extinse (├──, └──, │).
- Ignoră intrările `.` și `..`.

## Cerințe

- Compilator GCC (sau orice compilator C standard).
- Sistem de operare Linux/Unix (datorită utilizării `dirent.h`, `sys/stat.h`, `scandir`).

## Compilare

Pentru a compila programul, deschideți un terminal în directorul proiectului și rulați următoarea comandă:

```bash
gcc main.c -o tree
```

Aceasta va genera un executabil numit `tree`.

## Utilizare

Sintaxa de rulare a programului este:

```bash
./tree <cale_director>
```

### Exemplu

Pentru a afișa structura directorului curent:

```bash
./tree .
```

Sau pentru a afișa structura unui alt director (de exemplu `test_dir`):

```bash
./tree test_dir
```

## Structura Codului

- **main.c**: Fișierul sursă principal care conține logica programului.
  - `print_dir`: Funcția recursivă care parcurge directoarele și afișează structura.
  - `main`: Punctul de intrare, gestionează argumentele liniei de comandă.

## Autor

Acest program a fost realizat ca parte a unui exercițiu de utilizare a sistemelor de operare (USO).

## Varianta cu Adâncime Minimă (tree_mindepth)

Există și o variantă a programului care permite filtrarea afișării în funcție de adâncimea minimă a căilor. Aceasta este utilă pentru a vizualiza doar structurile de directoare adânci.

### Compilare

```bash
gcc tree_mindepth.c -o treemin
```

### Utilizare

Sintaxa este:

```bash
./treemin <cale_director> <adancime_minima>
```

### Exemplu

Pentru a afișa doar căile care au o adâncime de cel puțin 6 directoare:

```bash
./treemin . 6
```
