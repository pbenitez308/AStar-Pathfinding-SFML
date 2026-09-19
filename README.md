# A* Pathfinding Visualizer

Visualizador interactivo del algoritmo de búsqueda **A\*** desarrollado en **C++** utilizando **SFML 3**.

El programa permite definir un nodo inicial, un objetivo y obstáculos dentro de una cuadrícula, para posteriormente visualizar paso a paso cómo el algoritmo A* explora el espacio y encuentra la ruta óptima.

---

## Características

- Implementación del algoritmo A*
- Visualización paso a paso
- Heurística Manhattan
- Lista de nodos abiertos y cerrados
- Reconstrucción del camino final
- Creación interactiva de obstáculos
- Eliminación de obstáculos
- Detección de escenarios sin solución
- Interfaz gráfica desarrollada con SFML
- Arquitectura modular

---

## Algoritmo A*

A* evalúa cada nodo utilizando:

```text
f(n) = g(n) + h(n)