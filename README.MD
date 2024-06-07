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
    ./run_all.sh instance_name [verbose] [time_limit]     (Linux)
    run_all.bat instance_name [verbose] [time_limit]      (Windows)
    ```
    - `instance_name` : Nom de l'instance TSP à utiliser (sans l'extension `.tsp`).
    - `verbose` : Mode verbeux, ecrire "verbose" pour activer.
    - `time_limit` : Limite de temps d'exécution en secondes.
    Note : Pour utiliser la limite de temps, il faut également specifier le mode verbeux. Si on ne veut pas utiliser le mode verbeux, on peut alors mettre n'importe quelle valeur pour le mode verbeux.

    Pour exécuter un algorithme spécifique, utilisez la commande suivante depuis le répertoire racine du projet :
    ```
    .\src\little_algorithm\run.bat instance_name [verbose] [time_limit]    (Windows)
    ./src/little_algorithm/run.sh instance_name [verbose] [time_limit]      (Linux)
    ```

    ```
    .\src\tabu_search\run.bat instance_name [verbose] [time_limit]    (Windows)
    ./src/tabu_search/run.sh instance_name [verbose] [time_limit]      (Linux)
    ```

    ```
    .\src\genetic_algorithm\run.bat instance_name [verbose] [time_limit]    (Windows)
    ./src/genetic_algorithm/run.sh instance_name [verbose] [time_limit]      (Linux)
    ```

    Note : Pour utiliser la limite de temps, il faut également specifier le mode verbeux. Si on ne veut pas utiliser le mode verbeux, on peut alors mettre n'importe quelle valeur pour le mode verbeux.
3. **Nettoyage** : Utilisez le makefile pour nettoyer les fichiers objets, exécutables, et les résultats.
    ```
    make clean
    make clean_results
    ```

## Dépendances
- G++ pour la compilation (Linux)
- MinGW pour la compilation (Windows)
