# A comparison of genetic algorithms on the rastrigin function using elitism

The genetic algorithm runs on the rastrigin function with n taking the values 2, 15 and 30

There are 30 populations, each containing 100 induviduals that evolve over 10000 generations

| Rastrigin     | Average       | Minimum      | Time (seconds) |
|     :---:     | :---: | :---: | :---: |
| n=2           | 0.05289  | 0.00055  | 98  |
| n=2 + elitism(3) | 0.00175  | 0.00000  | 95  |
| n=15          | 153.88695  | 120.28596  | 734  |
| n=15 + elitism(3) | 121.97686  | 78.39811  | 673  |
| n=30          | 458.32394  | 413.39789  | 1447  |
| n=30 + elitism(3) | 376.79504  | 290.44547  | 1340  |
