# 3sem_laba2 
Крестики-нолики

Во второй лабораторной работе был реализован алгоритм решения задачи "Крестики-нолики", который принимает на вход текующую конфигурацию поля и с помощью результата минмаксного алгоритма находит оптимальный ход.

Ситуации содержатся в дереве, которое хранит варианты первого хода, затем в наследниках от каждого нода содержится следующие возможные конфигурации при таком ходе. 
Условие задачи усовершенствованно, и программа не просто выдает оптимальный ход, но реализован AI, с которым можно поиграть (его не получится выиграть).

Дополнительно реализован графический интерфейс с помощью QT редактора, добавлены счетчики побед, поражений, ничьей.
