# Projet IT45 - Printemps 2024

# Description
Ce projet traite le problème du voyageur de commerce (TSP) en utilisant différentes techniques de résolution : algorithme de Little, recherche tabou, et algorithme génétique.

## Structure du Projet
- **src/** : Contient les scripts des algorithmes.
- **data/** : Contient les instances TSP utilisées pour les tests.
- **results/** : Contient les résultats des exécutions des algorithmes.

## Instructions de Compilation et d'Exécution
1. **Compilation** : Utilisez le makefile pour compiler les programmes.
    ```
    make
    ```
2. **Exécution** : Utilisez le script `run_all.sh` (Linux) ou `run_all.bat` (Windows) pour exécuter tous les algorithmes.
    ```
    ./run_all.sh instance_name    (Linux)
    run_all.bat instance_name     (Windows)
    ```
    - `instance_name` : Nom de l'instance TSP à utiliser (sans l'extension `.tsp`).

## Dépendances
- GCC/G++ pour la compilation (Linux)
- MinGW pour la compilation (Windows)