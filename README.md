# Integrated Itinerary and Fare Engine

Práctica 2 - SI1002-7 Lenguajes y Paradigmas de la Computación (EAFIT, 2026-2)

## Integrantes
- Juan Camilo Gómez
- Samuel Córdoba

## Cómo compilar y ejecutar

```
g++ -std=c++17 -Wall -Wextra main.cpp -o fare_engine
./fare_engine
```
## Preguntas de sustentación

> El enunciado solicita responder dos preguntas de sustentación, pero en los resultados de aprendizaje plantea tres preguntas centrales. Por esta razón, respondemos las tres.

### 1. ¿Por qué una clase abstracta con funciones virtuales puras es preferible a una enumeración con `switch`?

Una clase abstracta con funciones virtuales puras permite que cada tipo de transporte defina su propio comportamiento dentro de su clase. Por ejemplo, `MetroLine`, `CableLine` y `TouristCable` implementan su propia tarifa y tipo de servicio mediante `baseFare()` y `serviceType()`.

Con un `enum` y un `switch`, cada vez que se agregara un nuevo medio de transporte habría que modificar las condiciones en varios lugares del programa. Eso aumenta el acoplamiento y el riesgo de errores.

Con polimorfismo, se puede agregar una nueva clase derivada sin modificar el código que ya trabaja con `TransportMode`. Cada objeto responde su propia política, evitando preguntar qué tipo es mediante `switch`, `dynamic_cast`, `typeid` o comparación de textos.

### 2. ¿Por qué un destructor virtual es un requisito de corrección y no una formalidad?

`TransportMode` se usa como clase base para almacenar objetos derivados mediante `unique_ptr<TransportMode>`. Aunque el puntero tenga tipo `TransportMode`, el objeto real puede ser un `MetroLine`, `CableLine` o `TouristCable`.

El destructor virtual garantiza que, al destruir el puntero base, se ejecute primero el destructor de la clase derivada real y después el destructor de la clase base.

Sin destructor virtual, podría ejecutarse solamente el destructor de `TransportMode`. Esto genera comportamiento indefinido y puede impedir que se liberen correctamente los recursos propios de la clase derivada. Por eso es una condición de corrección del programa.

### 3. ¿Qué aporta la Standard Template Library al reemplazar un ciclo manual por un algoritmo y una lambda?

La STL aporta algoritmos genéricos, reutilizables y probados, como `sort`, `accumulate`, `count_if`, `for_each` y `find_if`.

En lugar de escribir manualmente el recorrido de una colección, el algoritmo expresa la operación general y la lambda define únicamente la acción o condición específica del problema.

Por ejemplo, `accumulate` expresa que se desea sumar los cobros, `count_if` expresa que se desea contar los transbordos integrados y `sort` expresa que se desea ordenar los tramos. Esto reduce código repetido, mejora la claridad de la intención y evita errores frecuentes al escribir ciclos manuales.

Las lambdas permiten definir reglas pequeñas en el mismo lugar donde se usan, como comparar dos tramos por minuto o decidir si un transporte tiene más de ocho estaciones.