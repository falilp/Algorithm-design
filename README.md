# Diseño de Algoritmos
Realización de la practicas de la asignatura de Diseño de Algoritmos, consiste en la colocación de distintos tipos de elementos defensivos para poder realizar la defensa de tu base, esta colocación se aplicara mediante la creación de algoritmos y la mejor selección y orden de los mismos.

## 1. Descripción general
En el año 2100 ya no quedan suficientes recursos en la Tierra y la humanidad se lanza a la colonización de otros
planetas. Ya desde las primeras misiones descubrimos que no estamos solos en el universo y debemos luchar contra
otras civilizaciones alienígenas para conseguir dichos recursos. Los ucos son una raza de extraterrestres que pueblan
los planetas donde se encuentran los tan ansiados recursos y harán todo lo posible por evitar su extracción.
Hemos sido encomendados con la tarea de diseñar la estrategia defensiva de las bases que estableceremos en
los planetas asaltados. Las bases serán atacadas por alienígenas y nuestro objetivo es mantenerlas a salvo el mayor
tiempo posible para obtener así la mayor cantidad de recursos del planeta.

## 2. Reglas de juego
Cuando se descubre un planeta viable se le asocia un código numérico único y se procede al envío desde la tierra
de un conjunto de edificios defensivos para desplegar en dicho planeta. Estas defensas se utilizarán para evitar que
los ucos destruyan el centro de extracción de minerales, enviado también junto con ellas. Puesto que el enemigo
nos supera ampliamente en número, nuestro objetivo consiste en extraer la mayor cantidad posible de minerales
antes de que la base sea destruida. Se considerará que la base ha sido destruida cuando el centro de extracción de
minerales sea destruido por los ucos.
Las batallas contra los ucos se desarrollan en terrenos de juego cuadrados de dimensiones n × n. Dentro del
campo de batalla podemos encontrar tres tipos de entidades u objetos: obstáculos, defensas y unidades de UCOs.
Cada uno de estos tipos de entidades tiene sus propias características, si bien comparten algunas de ellas.

-Todos los objetos desplegados en el campo de batalla tienen asociado un código único.

-Todos los objetos tienen asociados un valor que indica el espacio ocupado por dicho objeto. Por simplicidad,
supondremos una forma más o menos cilíndrica de los objetos y bastará, por tanto, con indicar el valor de su
radio. En general y salvo que se indique lo contrario, ninguno de estos objetos podrá solaparse en el campo
de batalla con ningún otro objeto.
Todos los objetos tienen asociada una posición en el campo de batalla, que vendrá expresada por una coordenada tridimensional expresada como < x, y, z >. La esquina superior izquierda del terreno de juego se
define como la posición < 0, 0, 0 >. La esquina inferior derecha coincide con la coordenada < n, m, 0 >. 
Por simplicidad supondremos un terreno de juego plano, por lo que la tercera coordenada (z) siempre tendrá un
valor nulo
