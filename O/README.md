# O. Операционные системы
```diff
- Нет решения -
# полагаю, надо использовать метод сканирующей прямой #
```

| Правила                                    	|
|:-------------------------------------------:|
| ограничение по времени на тест: 2 секунды   |
| ограничение по памяти на тест: 64 мегабайта |
| ввод: стандартный ввод                      |
| вывод: стандартный вывод                    |

Васин жесткий диск состоит из ***M*** секторов. Вася последовательно устанавливал на него различные операционные системы следующим методом: он создавал новый раздел диска из последовательных секторов, начиная с сектора номер ***a<sub>i</sub>*** и до сектора ***b<sub>i</sub>*** включительно, и устанавливал на него очередную систему. При этом если очередной раздел хотя бы по одному сектору пересекается с каким-то ранее созданным разделом, то ранее созданный раздел «затирается», и операционная система, которая на него была установлена, больше не может быть загружена.

Напишите программу, которая по информации о том, какие разделы на диске создавал Вася, определит, сколько в итоге работающих операционных систем установлено и в настоящий момент работает на Васином компьютере.

## Входные данные
Сначала вводятся натуральное число ***M*** — количество секторов на жестком диске (***1 ≤ M ≤ 10<sup>9</sup>***) и целое число ***N*** — количество разделов, которое последовательно создавал Вася (***0 ≤ N ≤ 100000***). Далее идут ***N*** пар чисел ***a<sub>i</sub>*** и ***b<sub>i</sub>***, задающих номера начального и конечного секторов раздела (***1 ≤ a<sub>i</sub> ≤ b<sub>i</sub> ≤ M***).

## Выходные данные
Выведите одно число — количество работающих операционных систем на Васином компьютере.

## Примеры
входные данные
```
10
3
1 3
4 7
3 4
```
выходные данные
```
1
```
---
входные данные
```
10
4
1 3
4 5
7 8
4 6
```
выходные данные
```
3
```