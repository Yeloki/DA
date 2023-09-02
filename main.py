"""Напишите класс Волшебное Животное (MagicAnimal), экземпляр которого инициализируется с аргументами:
– имя;
– уровень магии;
– список целей, по умолчанию пустой.

Класс должен реализовать функциональность:
– add_goal(value) – добавить цель – цель добавляется в конец списка, если такой там не было;
– change_goal() – сменить цель – происходит сдвиг целей на одну позицию вправо: последняя становится первой, все остальные сдвигаются;
– add_magic(value) – добавить магию – уровень магии увеличивается на значение, которое может быть отрицательным. В этом случае уменьшение происходит только до 0, дальше не меняется;
– экземпляры класса можно сравнивать: сначала по уровню магии, затем по количеству целей, затем по имени по алфавиту; для этого должны быть реализованы методы сравнения;
str – возвращает строковое представление в виде:
Magic Animal <имя>, magic of <уровень>, first goal is <первая по списку цель>


__eq__() – для равенства ==
__ne__() – для неравенства !=
__lt__() – для оператора меньше <
__le__() – для оператора меньше или равно <=
__gt__() – для оператора больше >
__ge__() – для оператора больше или равно >=

"""
from collections import deque


class MagicAnimal:
    def __init__(self, name, magic_level, targets=None):
        if targets is None:
            targets = []
        self.targets = deque(targets)
        self.name = name
        self.magic_level = magic_level

    def add_goal(self, val):
        if val not in self.targets:
            self.targets.append(val)

    def change_goal(self):
        self.targets.rotate(1)

    def add_magic(self, val):
        self.magic_level += val
        self.magic_level = (self.magic_level if self.magic_level > 0 else 0)

    def __str__(self):
        return f"Magic Animal {self.name}, magic of {self.magic_level}, first goal is {self.targets[0]}"

    def __lt__(self, other):
        if self.magic_level != other.magic_level:
            return self.magic_level < other.magic_level
        if len(self.targets) != len(other.targets):
            return len(self.targets) < len(other.targets)
        return self.name < other.name

    def __eq__(self, other):
        return not self < other and not other < self

    def __ne__(self, other):
        return not self == other

    def __le__(self, other):
        return self < other or self == other

    def __gt__(self, other):
        return other < self

    def __ge__(self, other):
        return self > other or self == other


test1 = MagicAnimal('a', 1, [1, 2, 3])

test2 = MagicAnimal('a', 1, [1, 2, 3])
print(test2 == test1)
"""
xy — кортеж из 4 значений (x, y) — координаты нижнего левого угла рисунка, w и h — характерные размеры, относительно которых заданы остальные параметры рисунка;
ﬁll — кортеж из 4-х цветов в 16-ричном формате соответственно стены цирка, купол, дверь и шпиль.
Цирк представляет собой прямоугольник с треугольной крышей и треугольной дверью. Размеры указаны на рисунке. Шпиль — линия, толщиной 4 пикселя.
Метод рисует цирк и ничего не возвращает.
PIC
"""

from PIL import Image, ImageDraw


class CircusImageDraw(ImageDraw.ImageDraw):
    def circus(self, xy, fill):
        x, y, w, h = xy
        a, b, c, d = fill
        im = Image.new('RGB', (200, 200), '#000000')
        draw = ImageDraw.Draw(im)

        draw.rectangle((x, y, x + 2 * h, y - h), fill=a)
        draw.polygon((h - 0.5 * w + x, y, h + x, y - h, h + 0.5 * w + x, y), fill=b)
        draw.polygon((x, y - h, h + x, y - h - w, x + 2 * h, y - h), fill=c)
        draw.line((x + h, y - h - w, x + h, y - h - w - h // 4),
                  fill=d, width=4)
        im.save('result.png')

        # drw = CircusImageDraw(img)
        # drw.circus((10, 190, 68, 90), ('#ffffff', '#999999', '#666666', '#ffffff'))
        # img.save('result.png')
