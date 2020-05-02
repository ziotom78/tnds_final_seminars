---
author: "Maurizio Tomasi"
title: "Il linguaggio Python"
date: "Università degli Studi di Milano"
lang: italian
babel-lang: italian
---

# L'approccio di Python

-   Python nasce all’inizio degli anni 90, 20 anni dopo il C e 7 dopo
    il C++
-   Quando nasce il Python c’è la consapevolezza che i computer
    saranno sempre più veloci: programmi «lenti» sono sempre meno un
    problema
-   L'approccio di Python è completamente diverso rispetto al C++: non
    è più **compilato**, ma **interpretato**
-   Oggi la versione più diffusa è la 3: evitate la 2!
-   In campo scientifico si usa molto **Anaconda Python**

# Frattali di Julia in Python

```python
def julia(z, c, maxiter=256):
    iteridx = 0
    while (z.real**2 + z.imag**2 < 4) and (iteridx < maxiter):
        z = z * z + c
        iteridx += 1
        
    if iteridx == maxiter:
        return -1
    else:
        return iteridx
```

# Confronto tra C++ e Python

-   Il codice Python per generare un frattale di Julia è molto più
    semplice del codice C++
-   Il programma Python richiede **16 s** per generare un'immagine
    800×800 (in C++, 0.1 secondi!)

# Vantaggi di Python rispetto al C++

-   Si esegue il codice senza bisogno di compilare prima → più facile
    fare il debug
-   Non è necessario dichiarare variabili → codice più breve e veloce
    da scrivere
-   Non si usano i file header (`.h`) → meno file da gestire
-   Non si usano i Makefile → maggiore semplicità
-   Niente puntatori → minore possibilità di crash

# Svantaggi di Python

-   Se le variabili non hanno tipo, sono possibili molti errori
-   Gli errori capitano durante l’esecuzione, non durante la
    compilazione
-   **I programmi sono molto più lenti del C++!**

# Velocità di Python

-   Python non crea programmi nel linguaggio macchina della CPU, ma
    nell’assembler di una **macchina virtuale** (la «Python virtual
    machine»)
-   Questo codice non viene eseguito dalla CPU ma da un programma C,
    che lo converte **in fase di esecuzione** in una sequenza di
    istruzioni in linguaggio macchina
-   Vediamo come funziona in un esempio pratico

#

In C++, una istruzione come `x = a + b` può essere convertita in
Assembler così:

```asm
mov rax, QWORD PTR [rbp-24] ; rax = a
add rax, QWORD PTR [rbp-16] ; rax += b
mov QWORD PTR [rbp-8], rax  ; x = rax
```

oppure così:

```asm
movsd xmm0, QWORD PTR [rbp-24]  ; xmm0 = a
movsd xmm1, QWORD PTR [rbp-16]  ; xmm1 = b
addsd xmm0, xmm1                ; xmm0 += xmm1
movsd QWORD PTR [rbp-8], xmm0   ; x = xmm0
```

a seconda che le variabili `a`, `b` e `x` siano `int` o `double`.

#

Consideriamo ora questo programma Python:

```python
def add(a, b):
    return a + b
	
print(add(1, 3))      # Result: 4
print(add(1.0, 3.0))  # Result: 4.0
print(add('a', 'b'))  # Result: 'ab'
```

Come può Python compilare in un linguaggio assembler la funzione
`add`, visto che la somma può assumere significati diversi?

# Compilazione e Python

-   In Python, l'istruzione `x = a + b` viene **sempre** compilata
    così:

    ```sh
    load_fast   0 # 0 stands for a
    load_fast   1 # 1 stands for b
    binary_add    # sum the last two nums
    store_fast  2 # 2 stands for x
    ```

-   Questi comandi assumono che ci sia un vettore di elementi
    (chiamato *stack*) che venga mantenuto durante l'esecuzione, e che
    `load_fast` e `store_fast` aggiungano e tolgano elementi in coda
    al vettore.
    
-   Istruzioni come `binary_add` tolgono uno o più elementi in coda al
    vettore, fanno un'operazione su di essi, e mettono il risultato in
    coda al vettore

#

Per eseguire il file `test.py`, occorre sempre chiamare `python3`:

```sh
python3 test.py
```

Il programma `python3` è scritto in C, ed è più o meno fatto così:

```c
int main(int argc, const char argv[argc + 1]) {
    initialize();

    PyProgram * prog = compile_to_bytecode(argc, argv);
    while(1) { /* Run commands in sequence, like a real CPU */
        PyCommand * command = get_next_bytecode(prog);
        if (! run_command(command))
            break;
    }
    return 0;
}
```

# Cosa fa `run_command`

-   La funzione `run_command` esegue *una* istruzione, e ogni volta
    che viene invocata deve capire come operare in base al tipo di
    dato.

-   Verosimilmente, a seconda del comando che deve eseguire,
    `run_command` chiama una funzione C che gestisce l'esecuzione di
    quel particolare comando (`load_fast`, `store_fast`, `binary_add`,
    …)


# 
Questa è una possibile implementazione per `binary_add`:

```c
void binary_add(PyObject * val1,
                PyObject * val2,
                PyObject * result) {	
	if (isinteger(val1) && isinteger(val2)) {
        /* Sum two integers */
		int v1 = get_integer(val1);
		int v2 = get_integer(val2);
		result.set_type(PY_INTEGER)
		result.set_integer(v1 + v2);
	} else if (isreal(val1) && isreal(val2)) {
		/* Sum two floating-point numbers */
    } else {
        /* ... */
    }
}
```

# Linguaggi interpretati

-   Di per sé, un linguaggio interpretato non deve essere
    necessariamente più lento di un linguaggio compilato
-   Controesempi: in certi casi, Java (che usa una virtual machine
    come Python) è più veloce del C++
-   Molte implementazioni dei linguaggi LISP e Scheme (interpretati,
    con tipi dinamici come Python) sono ordini di grandezza più veloci
    di Python
-   Esistono compilatori Python poco usati (es., pypy, jython) che
    producono codice più veloce

# Comodità di Python

-   Python non viene certo usato per scrivere codice *veloce*, ma per
    scrivere codice **rapidamente**!
-   A differenza del C++, il linguaggio supporta molte funzionalità di
    alto livello
    
# Esempio

-   Supponiamo di avere un file, `test.txt`, contenente questi dati:

    ```
    # This is a comment
    #
    # sensor temperature
    upper_flange 301.76
    lower_flange   270.1
      horn         290.81
  
    detector        85.3
    ```

-   Esso contiene delle temperature registrate da termometri
    installati in uno strumento
    
-   Vogliamo scrivere un programma che stampi a video i nomi dei
    sensori, ordinati secondo la temperatura dal più freddo al più
    caldo. Il codice deve ignorare spazi, commenti e linee vuote
    
# Soluzione dell'esercizio

```python
with open("test.txt", "rt") as inpf:
    lines = [x.strip() for x in inpf.readlines()]

# Remove from "lines" empty lines and comments
lines = [x for x in lines if x != "" and x[0] != "#"]

# Split each line in two
pairs = [x.split() for x in lines]

for sensor, temp in sorted(pairs, key=lambda x: float(x[1])):
    print(f"{sensor:20} (T = {temp} K)")
```

```
detector             (T = 85.3 K)
lower_flange         (T = 270.1 K)
horn                 (T = 290.81 K)
upper_flange         (T = 301.76 K)
```

# Quando usare Python?

-   Se un programma non richiede molti calcoli complessi, Python è
    solitamente la scelta migliore
-   Se un programma Python è 100 volte più lento di un programma C++,
    ma completa sempre l’esecuzione in 0,1 secondi, vale la pena
    velocizzarlo?
-   Scrivere programmi in Python è molto più veloce che scriverli in
    C++

# Python nel calcolo scientifico

-   È possibile usare Python per simulazioni Monte Carlo? O per
    calcoli numerici su milioni di elementi?
-   Python permette di invocare funzioni scritte in C e in Fortran
-   Negli anni sono state sviluppate librerie Python molto potenti per
    il calcolo scientifico

# NumPy

-   Usa il concetto di broadcast: un’operazione su un array viene
    propagata su tutti gli elementi dell’array
-   La propagazione viene fatta da codice C o Fortran, ed è quindi
    velocissima
-   Si raggiunge una buona velocità, a volte confrontabile con C++ e Fortran

```python
import numpy as np

r = np.random.randn(10_000) # Generate 10.000 random numbers
print(f"Average value: {r.mean()}")
```

# 

Usando NumPy, i cicli `for` diventano impliciti (sono implementati in
`numpy.linspace` e in `numpy.sum`, che sono scritti in C/Fortran):

<table width="100%">
<tr>
<td>
**C++**
</td>
<td>
**Python**
</td>
</tr>
<tr>
<td>
```c++
double midpoint(double fn(double),
                double a, double b,
                int nstep) {
    double sum = 0.;
    double h = (b - a) / nstep;
    for (int i = 0; i < nstep; ++i) {
        sum += fn(a + (i + 0.5) * h);
    }
    return sum * h;
}
```
</td>
<td>
```python
import numpy

def midpoint(fn, a, b, nsteps):
	h = (b - a) / nsteps
	x = numpy.linspace(a + h/2, b - h/2, nsteps)
	return numpy.sum(fn(x)) * h
```
</td>
</tr>
</table>

# Altre possibilità

-   [Numba](https://numba.pydata.org/) è una libreria che compila
    codice Python in codice assembler, usando LLVM. Supporta anche
    architetture GPU, ed è molto interessante:
    -   Semplice da usare
    -   Il codice prodotto è molto veloce
    -   Non richiede di usare altri linguaggi oltre a Python
-   [f2py](https://www.numfys.net/howto/F2PY/) (parte di NumPy)
    compila routine in Fortran e in C (usando ad esempio GCC), e le
    rende chiamabili da Python
-   [Cython](https://cython.org/) è un compilatore che compila una
    variante di Python in codice C, che viene poi compilato da GCC o
    `clang` in linguaggio macchina

# Frattali di Julia usando Numba

```python
from numba import njit

@njit  # This is the *only* change!
def julia(z, c, maxiter=256):
    iteridx = 0
    while (z.real ** 2 + z.imag ** 2 < 4) and (iteridx < maxiter):
        # Python supports complex numbers
        z = z * z + c
        iteridx += 1

    if iteridx == maxiter:
        return -1
    else:
        return iteridx
```

# Frattali di Julia usando Numba

-   Il confronto tra le prestazioni delle tre soluzioni è impressionante:

    Linguaggio      Velocità
    --------------  ---------
    Python          16.3 s
    Python+Numba     1.1 s
    C++              0.1 s

-   L'esempio con Numba può essere ulteriormente ottimizzato, portando
    il codice a velocità ancora più simili a quelle del C++

# Librerie scientifiche Python

-   [Jupyter](https://jupyter.org/): scrittura di codice in modalità
    interattiva
-   [SciPy](https://www.scipy.org/): libreria scientifica per
    integrali, interpolazioni, ricerca di zeri, etc.
-   [Pandas](https://pandas.pydata.org/): gestione di dati tabellari
    (carica anche file Excel!)
-   [Matplotlib](https://matplotlib.org/): creazione di grafici.
    Guardate gli [esempi](https://matplotlib.org/gallery/index.html)
    per lustrarvi gli occhi
-   [Altair](https://altair-viz.github.io/): libreria grafica come
    Matplotlib, più orientata alla data analysis e al machine learning
    ([esempi](https://altair-viz.github.io/gallery/index.html))
-   [PyTorch](https://pytorch.org/): libreria di reti neurali per
    machine learning
