# renga_3d-export
[In progress now!] Plugin to convert renga's model to 3D-data formats (now - for Autodesk Navisworks - NWC and FBX)

## External dependencies (Внешние зависимости):
1. Renga SDK (all header's file (in "Renga_imprort.h") and RengaCOMAPI.tlb). Download SDK [here](https://rengabim.com/sdk/);
2. Autodesk Navisworks SDK, nwcreate-lib; download SDK [here](https://www.autodesk.com/developer-network/platform-technologies/navisworks). I am using 2022 version;
3. Autodesk FBX SDK (2019.5);

## About using (Об использовании):
Будет дополнено

## О поддержке объектов
- экспорт идет только объектов из 3D-представления, для которых есть методы получения геометрии;
- не для всех категорий объектов реализована точная передача цвета ввиду сложности описывающих интерфейсов. НЕ реализована логика назначения цвета по стилю (например, для трасс сетей) а также для объектов входящих в Сборки. Для многослойных материалов назначается первый в списке цвет. Для элементов, имеюших несколько GridType пока назначается статичный цвет. Для GridType::Glass назначается коэффициент прозрачности = 0.6;
- пока не реализована поддержка расчетных свойств и параметрических свойств;

## О поддерживаемых форматах экспорта
### Autodesk Navisworks
Формирует файл NWC с сортировкой объектов по уровням (пока без дополнительных валоженных типов).

## Планы на будущее
- SketchUp, \*.skp;
- GMSH, \*.msh;
- CityGML, \*.gml;
- Autodesk(?) FBX, \*.fbx;
- Rhino, \*.3dm;
- Chronos group GLTF, \*.gltf/\*.glb;
