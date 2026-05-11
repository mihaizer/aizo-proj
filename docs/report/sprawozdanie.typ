#import "generated-data.typ": generated-task-summary

#set page(margin: 2.2cm, paper: "a4")
#set text(font: "New Computer Modern", size: 11pt, lang: "pl")
#set heading(numbering: "1.")
#set par(justify: true, leading: 0.7em)

#show heading.where(level: 1): set text(size: 15pt, weight: "bold")
#show heading.where(level: 2): set text(size: 12.5pt, weight: "bold")
#show raw.where(block: true): it => block(
  fill: luma(242),
  inset: 8pt,
  radius: 4pt,
  width: 100%,
  text(font: "Menlo", size: 9pt, it),
)

#let summary-row(task-id) = {
  for row in generated-task-summary {
    if row.task-id == task-id {
      return row
    }
  }
  panic("Missing generated row for task " + str(task-id))
}

#let fmt-us(value) = [#value us]
#let fmt-avg(value) = [#value us]
#let cell(body) = table.cell(inset: 4pt, body)

#align(center)[
  #v(1.2cm)
  #text(size: 17pt, weight: "bold")[Algorytmy i złożoność obliczeniowa - Projekt]
  #v(0.5cm)
  #text(size: 15pt, weight: "bold")[Sprawozdanie]
  #v(0.5cm)
  #text(size: 13pt)[Zadanie projektowe nr 1]
  #v(1.2cm)
  #text(size: 14pt, weight: "bold")[Analiza algorytmów sortowania i wpływu struktury danych na czas wykonania]
  #v(1.4cm)
  #text(size: 12pt)[Mykhailo Tsonyev]
  #text(size: 12pt)[nr albumu 284497]
  #v(0.6cm)
  #text(size: 11pt)[Prowadzący kurs: ]
  #linebreak()
  #text(size: 11pt)[Termin zajęć: TN wtorek 9:15]
  #linebreak()
  #text(size: 11pt)[Termin oddania zadania: 12 maja 2026]
]

#pagebreak()

= Wstęp

Celem projektu było zaimplementowanie programu sortującego dane rosnąco oraz przeprowadzenie pomiarów czasu działania dla zakresu wymaganego na ocenę 3.0. Wymagany zakres obejmował trzy algorytmy sortowania: `CocktailSort`, `MergeSort` i `InsertionSort`, a także dwie własne struktury liniowe: `DynamicArray` oraz `SinglyLinkedList`. Oprócz samego sortowania program miał również sprawdzać poprawność wyniku, walidować parametry wejściowe oraz zapisywać wyniki pomiarów do pliku CSV.

Część eksperymentalna została podzielona na trzy główne badania. `Badanie A` dotyczy wpływu liczebności zbioru i obejmuje wszystkie kombinacje trzech algorytmów, dwóch struktur i czterech rozmiarów wejścia. `Badanie B` dotyczy wpływu rozkładu danych wejściowych i obejmuje osiem benchmarków dla `MergeSort` wykonanych na obu strukturach oraz dla czterech rozkładów danych. `Badanie C` dotyczy wpływu typu danych i obejmuje trzy benchmarki dla `MergeSort`, struktury `DynamicArray`, rozmiaru `25000` i typów `int`, `float` oraz `unsigned int`.

W interpretacji wyników korzystano z klasycznych pojęć złożoności obliczeniowej `O(n^2)` oraz `O(n log n)` opisanych w literaturze algorytmicznej [1][4][5]. Program uruchamiano na komputerze MacBook Air M1 z 8 GB pamięci RAM. Dane eksperymentalne pochodzą z pliku `results/research_data.csv`, który zawiera 35 benchmarków, a każdy benchmark składa się z 50 iteracji.

= Opis implementacji

Implementacja używa szablonów, dzięki czemu te same algorytmy mogą pracować na różnych typach danych bez powielania kodu. Wspólny interfejs struktur zapewnia klasa `IStructure<T>`, która udostępnia metody `size()`, `operator[]`, `pushBack()` oraz `swap()`. Taki układ pozwala stosować te same algorytmy dla tablicy i listy tam, gdzie jest to uzasadnione, a tam, gdzie koszt dostępu indeksowego byłby zbyt duży, przygotować osobną implementację dla `SinglyLinkedList` [2][5].

== Struktura DynamicArray

`DynamicArray<T>` przechowuje dane w ręcznie zarządzanym buforze pamięci `data`, a dodatkowo zapisuje `currentSize` oraz `capacity`. Struktura udostępnia dostęp indeksowy przez `operator[]`, zwraca bieżącą liczbę elementów przez `size()`, pozwala dopisać element na koniec przez `pushBack()` i zamieniać miejscami dwa elementy przez `swap(i, j)`. Dodatkowo posiada konstruktor kopiujący, operator przypisania oraz destruktor, które odpowiadają za poprawne zarządzanie pamięcią.

Z punktu widzenia algorytmów najważniejsze jest to, że `DynamicArray` oferuje szybki dostęp do elementu o zadanym indeksie. Dzięki temu klasyczne wersje `CocktailSort`, `MergeSort` i `InsertionSort` mogą korzystać z indeksów bez dodatkowego narzutu związanego z przechodzeniem po strukturze [2][5].

== Struktura SinglyLinkedList

`SinglyLinkedList<T>` przechowuje elementy w węzłach `Node`, z których każdy zawiera wartość `value` i wskaźnik `next` na kolejny węzeł. Struktura posiada wskaźniki `head` i `tail`, dzięki czemu zna początek i koniec listy, a także pole `currentSize`, które przechowuje liczbę elementów. Publiczne operacje obejmują `pushBack()`, `size()`, `operator[]` oraz `swap(i, j)`. W części prywatnej znajdują się też metody pomocnicze `clear()`, `copyFrom()` i `nodeAt(index)`.

W przeciwieństwie do `DynamicArray`, dostęp do elementu o indeksie `i` nie jest tutaj bezpośredni. Metoda `operator[]` wykorzystuje funkcję `nodeAt(index)`, która przechodzi liniowo od `head` do żądanego węzła. Z tego powodu nie wszystkie algorytmy powinny używać listy przez `operator[]`, bo przy dużej liczbie odwołań indeksowych koszt wykonania wzrasta bardzo wyraźnie [2][5].

== Algorytm CocktailSort

`CocktailSort` wykonuje dwa przejścia w obrębie aktualnego zakresu danych: najpierw od lewej do prawej, przesuwając większe elementy ku końcowi, a następnie od prawej do lewej, przesuwając mniejsze elementy ku początkowi. Dla `DynamicArray` algorytm korzysta bezpośrednio z `operator[]` i `swap()`.

Dla `SinglyLinkedList` przygotowano osobną implementację. Zamiast wielokrotnie wywoływać `nodeAt(index)` przez `operator[]`, algorytm najpierw buduje tymczasową tablicę wskaźników do kolejnych węzłów listy. Następnie porównuje i zamienia wartości węzłów, przechodząc po tej tablicy wskaźników. Gdyby wykorzystać listę wyłącznie przez `operator[]`, pojedynczy dostęp miałby koszt liniowy, a całe sortowanie otrzymałoby bardzo duży dodatkowy narzut, praktycznie zbliżający się do zachowania sześciennie rosnącego względem rozmiaru wejścia. Osobna implementacja ogranicza ten problem i zachowuje naturalny dla `CocktailSort` charakter kwadratowy [3][5].

== Algorytm MergeSort

`MergeSort` działa według schematu dziel i zwyciężaj. Dla `DynamicArray` algorytm rekurencyjnie dzieli zakres indeksów na dwie połowy, sortuje każdą z nich, a następnie scala obie uporządkowane części. W czasie scalania używany jest jeden dodatkowy bufor pomocniczy, czyli tymczasowa tablica `buffer`, do której przepisywane są elementy w trakcie łączenia dwóch posortowanych fragmentów. Bufor ten jest alokowany tylko raz dla całego sortowania, a nie przy każdym poziomie rekurencji [1][5].

Dla `SinglyLinkedList` zastosowano osobną implementację opartą na wskaźnikach. Lista jest dzielona na dwie połowy metodą szybkiego i wolnego wskaźnika, a następnie scalana przez przepinanie pól `next`, bez kopiowania całej listy do osobnej struktury i bez używania `operator[]`. Taki wariant lepiej odpowiada właściwościom listy jednokierunkowej i pozwala utrzymać oczekiwaną złożoność `O(n log n)` [1][2][5].

== Algorytm InsertionSort

`InsertionSort` przetwarza dane od lewej do prawej. W każdej iteracji pobiera bieżący element, porównuje go z elementami poprzedzającymi i przesuwa większe wartości o jedno miejsce w prawo, aż znajdzie pozycję, w którą można wstawić rozpatrywany element. Dla `DynamicArray` operacja ta jest realizowana bezpośrednio na indeksach tablicy [1][5].

Dla `SinglyLinkedList` zastosowano drugą wersję algorytmu. Zamiast próbować przesuwać elementy przez kosztowny dostęp indeksowy, implementacja buduje nową uporządkowaną część listy. Kolejne węzły są wyjmowane z listy wejściowej i wstawiane w odpowiednie miejsce już posortowanej części. Dzięki temu algorytm korzysta z naturalnych operacji listowych i nie opiera się na `operator[]` [2][5].

= Metodyka badań

W niniejszym sprawozdaniu termin `główne badanie` odnosi się do trzech części eksperymentu opisanych w treści zadania: `Badanie A`, `Badanie B` i `Badanie C`. Wewnątrz każdego głównego badania wykonywano pojedyncze benchmarki, czyli uruchomienia programu z ustalonym zestawem parametrów: algorytmem, strukturą danych, typem danych, rozkładem wejścia i rozmiarem zbioru. Każdy benchmark był następnie powtarzany 50 razy. Jedno takie powtórzenie jest w tym sprawozdaniu nazywane `iteracją`.

W pliku CSV zapisano surowy czas każdej iteracji. Ostatni wiersz każdego bloku 50 iteracji zawiera wartości `min_us`, `avg_us` i `max_us`, obliczone dla danego benchmarku. Analiza wyników została wykonana blokami po 50 wierszy, zgodnie z kolejnością uruchamiania benchmarków w skrypcie badań. Taki sposób interpretacji jest konieczny, ponieważ część benchmarków z różnych głównych badań ma identyczne parametry logiczne, ale należy do innych części eksperymentu.

Rozkład benchmarków pomiędzy główne badania był następujący:

- `Badanie A` obejmuje 24 benchmarki:
  każda kombinacja `CocktailSort`, `MergeSort`, `InsertionSort` z `DynamicArray` i `SinglyLinkedList` dla rozmiarów `5000`, `10000`, `25000` i `50000`.
- `Badanie B` obejmuje 8 benchmarków:
  `MergeSort` dla `DynamicArray` i `SinglyLinkedList`, a dla każdej struktury rozkłady `random`, `ascending`, `ascending50Per` i `descending`.
- `Badanie C` obejmuje 3 benchmarki:
  `MergeSort` dla `DynamicArray` i rozmiaru `25000` przy typach `int`, `float` i `unsigned int`.

= Wyniki badań

== Badanie A

Badanie A sprawdza wpływ liczebności zbioru na czas działania algorytmów. W tej części wykonano 24 benchmarki, czyli wszystkie kombinacje trzech algorytmów, dwóch struktur i czterech rozmiarów wejścia. Już na poziomie wykresów widać, że `MergeSort` skaluje się znacznie lepiej niż `CocktailSort` i `InsertionSort`, zwłaszcza dla większych rozmiarów danych, co jest zgodne z klasyczną analizą złożoności [1][4][5].

#figure(
  image("/results/plots/badanie_A_array.png", width: 100%),
  caption: [Badanie A dla struktury `DynamicArray`.],
)

#figure(
  image("/results/plots/badanie_A_singleList.png", width: 100%),
  caption: [Badanie A dla struktury `SinglyLinkedList`.],
)

#figure(
  kind: table,
  align(center)[
    #table(
      columns: 5,
      stroke: 0.6pt,
      [Algorytm], [Rozmiar], [Min [us]], [Avg [us]], [Max [us]],
      [CocktailSort], [5000], [#summary-row(1).min-us], [#summary-row(1).avg-us], [#summary-row(1).max-us],
      [CocktailSort], [10000], [#summary-row(2).min-us], [#summary-row(2).avg-us], [#summary-row(2).max-us],
      [CocktailSort], [25000], [#summary-row(3).min-us], [#summary-row(3).avg-us], [#summary-row(3).max-us],
      [CocktailSort], [50000], [#summary-row(4).min-us], [#summary-row(4).avg-us], [#summary-row(4).max-us],
      [MergeSort], [5000], [#summary-row(9).min-us], [#summary-row(9).avg-us], [#summary-row(9).max-us],
      [MergeSort], [10000], [#summary-row(10).min-us], [#summary-row(10).avg-us], [#summary-row(10).max-us],
      [MergeSort], [25000], [#summary-row(11).min-us], [#summary-row(11).avg-us], [#summary-row(11).max-us],
      [MergeSort], [50000], [#summary-row(12).min-us], [#summary-row(12).avg-us], [#summary-row(12).max-us],
      [InsertionSort], [5000], [#summary-row(17).min-us], [#summary-row(17).avg-us], [#summary-row(17).max-us],
      [InsertionSort], [10000], [#summary-row(18).min-us], [#summary-row(18).avg-us], [#summary-row(18).max-us],
      [InsertionSort], [25000], [#summary-row(19).min-us], [#summary-row(19).avg-us], [#summary-row(19).max-us],
      [InsertionSort], [50000], [#summary-row(20).min-us], [#summary-row(20).avg-us], [#summary-row(20).max-us],
    )
  ],
  caption: [Czas sortowania w zależności od liczebności zbioru dla struktury `DynamicArray`.],
)

#figure(
  kind: table,
  align(center)[
    #table(
      columns: 5,
      stroke: 0.6pt,
      [Algorytm], [Rozmiar], [Min [us]], [Avg [us]], [Max [us]],
      [CocktailSort], [5000], [#summary-row(5).min-us], [#summary-row(5).avg-us], [#summary-row(5).max-us],
      [CocktailSort], [10000], [#summary-row(6).min-us], [#summary-row(6).avg-us], [#summary-row(6).max-us],
      [CocktailSort], [25000], [#summary-row(7).min-us], [#summary-row(7).avg-us], [#summary-row(7).max-us],
      [CocktailSort], [50000], [#summary-row(8).min-us], [#summary-row(8).avg-us], [#summary-row(8).max-us],
      [MergeSort], [5000], [#summary-row(13).min-us], [#summary-row(13).avg-us], [#summary-row(13).max-us],
      [MergeSort], [10000], [#summary-row(14).min-us], [#summary-row(14).avg-us], [#summary-row(14).max-us],
      [MergeSort], [25000], [#summary-row(15).min-us], [#summary-row(15).avg-us], [#summary-row(15).max-us],
      [MergeSort], [50000], [#summary-row(16).min-us], [#summary-row(16).avg-us], [#summary-row(16).max-us],
      [InsertionSort], [5000], [#summary-row(21).min-us], [#summary-row(21).avg-us], [#summary-row(21).max-us],
      [InsertionSort], [10000], [#summary-row(22).min-us], [#summary-row(22).avg-us], [#summary-row(22).max-us],
      [InsertionSort], [25000], [#summary-row(23).min-us], [#summary-row(23).avg-us], [#summary-row(23).max-us],
      [InsertionSort], [50000], [#summary-row(24).min-us], [#summary-row(24).avg-us], [#summary-row(24).max-us],
    )
  ],
  caption: [Czas sortowania w zależności od liczebności zbioru dla struktury `SinglyLinkedList`.],
)

Przykładowo dla `DynamicArray` i rozmiaru `50000` średni czas `MergeSort` wyniósł #fmt-avg(summary-row(12).avg-us), podczas gdy `CocktailSort` osiągnął #fmt-avg(summary-row(4).avg-us), a `InsertionSort` #fmt-avg(summary-row(20).avg-us). Oznacza to bardzo dużą różnicę pomiędzy algorytmem `O(n log n)` a algorytmami o dominującym zachowaniu `O(n^2)` [1][4][5].

== Badanie B

Badanie B dotyczy wpływu rozkładu danych wejściowych. W tej części wykonano 8 benchmarków: po cztery dla `DynamicArray` i `SinglyLinkedList`. Każdy benchmark używał `MergeSort`, a zmieniano jedynie rozkład danych wejściowych. Wykresy i tabela pokazują, że dla tego algorytmu rozkład wejścia wpływa na wynik dużo słabiej niż sama liczebność zbioru w badaniu A.

#figure(
  image("/results/plots/badanie_B_array.png", width: 100%),
  caption: [Badanie B dla `MergeSort` i struktury `DynamicArray`.],
)

#figure(
  image("/results/plots/badanie_B_singleList.png", width: 100%),
  caption: [Badanie B dla `MergeSort` i struktury `SinglyLinkedList`.],
)

#figure(
  kind: table,
  align(center)[
    #table(
      columns: 5,
      stroke: 0.6pt,
      [Struktura], [Rozklad danych], [Min [us]], [Avg [us]], [Max [us]],
      [DynamicArray], [random], [#summary-row(25).min-us], [#summary-row(25).avg-us], [#summary-row(25).max-us],
      [DynamicArray], [ascending], [#summary-row(26).min-us], [#summary-row(26).avg-us], [#summary-row(26).max-us],
      [DynamicArray], [ascending50Per], [#summary-row(27).min-us], [#summary-row(27).avg-us], [#summary-row(27).max-us],
      [DynamicArray], [descending], [#summary-row(28).min-us], [#summary-row(28).avg-us], [#summary-row(28).max-us],
      [SinglyLinkedList], [random], [#summary-row(29).min-us], [#summary-row(29).avg-us], [#summary-row(29).max-us],
      [SinglyLinkedList], [ascending], [#summary-row(30).min-us], [#summary-row(30).avg-us], [#summary-row(30).max-us],
      [SinglyLinkedList], [ascending50Per], [#summary-row(31).min-us], [#summary-row(31).avg-us], [#summary-row(31).max-us],
      [SinglyLinkedList], [descending], [#summary-row(32).min-us], [#summary-row(32).avg-us], [#summary-row(32).max-us],
    )
  ],
  caption: [Czas sortowania w zależności od początkowego rozkładu elementów.],
)

Dla `DynamicArray` średni czas dla rozkładu `random` wyniósł #fmt-avg(summary-row(25).avg-us), dla `ascending` #fmt-avg(summary-row(26).avg-us), dla `ascending50Per` #fmt-avg(summary-row(27).avg-us), a dla `descending` #fmt-avg(summary-row(28).avg-us). Analogiczny trend widać dla `SinglyLinkedList`, choć poziom czasów jest tam inny ze względu na inną organizację danych.

== Badanie C

Badanie C analizuje wpływ typu danych przy zachowaniu tego samego algorytmu, tej samej struktury oraz tego samego rozmiaru wejścia. W tej części wykonano 3 benchmarki: dla `int`, `float` i `unsigned int`. Wyniki pokazują, że w tej konfiguracji dominujący wpływ ma sam algorytm i organizacja danych, a nie drobne różnice pomiędzy badanymi typami [1][5].

#figure(
  image("/results/plots/badanie_C_types.png", width: 88%),
  caption: [Badanie C dla `MergeSort`, `DynamicArray` i rozmiaru `25000`.],
)

#figure(
  kind: table,
  align(center)[
    #table(
      columns: 4,
      stroke: 0.6pt,
      [Typ danych], [Min [us]], [Avg [us]], [Max [us]],
      [int], [#summary-row(33).min-us], [#summary-row(33).avg-us], [#summary-row(33).max-us],
      [float], [#summary-row(34).min-us], [#summary-row(34).avg-us], [#summary-row(34).max-us],
      [unsigned int], [#summary-row(35).min-us], [#summary-row(35).avg-us], [#summary-row(35).max-us],
    )
  ],
  caption: [Czas sortowania w zależności od typu danych.],
)

Średnie czasy wyniosły odpowiednio:

- `int`: #fmt-avg(summary-row(33).avg-us)
- `float`: #fmt-avg(summary-row(34).avg-us)
- `unsigned int`: #fmt-avg(summary-row(35).avg-us)

= Analiza wyników

W analizie wyników warto rozdzielić dwie grupy algorytmów. `CocktailSort` oraz `InsertionSort` należą tutaj do algorytmów o dominującym zachowaniu kwadratowym `O(n^2)`, natomiast `MergeSort` zachowuje złożoność `O(n log n)` [1][4][5]. Tę różnicę widać bezpośrednio w badaniu A: wraz ze wzrostem rozmiaru wejścia czasy `CocktailSort` i `InsertionSort` rosną znacznie szybciej niż czasy `MergeSort`.

Na poziomie implementacji taki wynik jest w pełni uzasadniony. `MergeSort` dla `DynamicArray` wykonuje podział zakresu i scalanie uporządkowanych fragmentów z użyciem jednego dodatkowego bufora, czyli tymczasowej tablicy pomocniczej. Dla `SinglyLinkedList` ten sam algorytm nie wykonuje kosztownego dostępu indeksowego, lecz dzieli listę na połowy i scala ją przez przepinanie wskaźników `next`. Dzięki temu obie wersje zachowują się zgodnie z oczekiwaną teorią [1][2][5].

`CocktailSort` oraz `InsertionSort` są znacznie bardziej wrażliwe na wzrost liczby elementów. W przypadku `SinglyLinkedList` autor implementacji musiał dodatkowo uważać na koszt dostępu do elementu pod indeksem. Z tego powodu `CocktailSort` otrzymał osobną wersję wykorzystującą tablicę wskaźników do węzłów, a `InsertionSort` osobną wersję budującą uporządkowaną część listy przez wstawianie węzłów we właściwe miejsce. Bez takich zmian wykorzystanie samego `operator[]` prowadziłoby do bardzo dużego dodatkowego narzutu [2][5].

W badaniu B rozkład danych nie zmienia jakościowo zachowania `MergeSort`, ale wpływa na konkretne czasy wykonania. W szczególności przypadek `ascending50Per` bywa wolniejszy od `descending`, mimo że oba należą do tego samego głównego badania. Wynika to z tego, że dane `ascending50Per` nie są całkowicie uporządkowane: połowa wejścia jest posortowana, a reszta pozostaje losowa. Przy scalaniu takich fragmentów algorytm wykonuje więcej nieregularnych porównań niż w bardziej regularnym przypadku całkowicie malejącym. Wpływ mają tu również szczegóły implementacyjne i lokalność pamięci [1][5].

W badaniu C różnice pomiędzy `int`, `float` i `unsigned int` są zauważalne, ale dużo mniejsze niż różnice wynikające ze zmiany algorytmu w badaniu A. Oznacza to, że w tej konfiguracji koszt sortowania jest zdominowany głównie przez liczbę wykonywanych operacji oraz sposób organizacji struktury danych, a nie przez sam wybór jednego z trzech prostych typów liczbowych [1][4][5].

= Wnioski

Najważniejszy wniosek z przeprowadzonych eksperymentów jest taki, że `MergeSort` wyraźnie dominuje przy większych rozmiarach danych. Już w badaniu A widać, że przy `50000` elementów różnica pomiędzy `MergeSort` a pozostałymi algorytmami nie jest kosmetyczna, lecz bardzo duża. Oznacza to, że przy większych wejściach przewaga złożoności `O(n log n)` przekłada się bezpośrednio na praktyczny czas działania.

Drugi istotny wniosek dotyczy samej struktury danych i sposobu implementacji. `SinglyLinkedList` wymagała osobnych wersji części algorytmów, ponieważ prosty dostęp indeksowy nie odpowiada naturze listy jednokierunkowej. Wyniki pokazują więc nie tylko różnice między teorią poszczególnych algorytmów, lecz także to, że dobór sposobu implementacji do właściwości struktury danych ma realny wpływ na końcowy rezultat.

Trzeci wniosek płynący z badań B i C jest bardziej praktyczny. Zmiana rozkładu wejścia lub prostego typu liczbowego wpływa na wyniki, ale wpływ ten jest słabszy niż zmiana samego algorytmu. W badanym zakresie to właśnie wybór algorytmu i dopasowanie go do struktury danych okazały się najważniejsze dla osiągnięcia dobrych czasów sortowania.

= Literatura

[1] Maciej M. Sysło, _Algorytmy_, Helion, 2016.

[2] L. Banachowski, K. Diks, W. Rytter, _Algorytmy i struktury danych_, Wydawnictwo Naukowe PWN, Warszawa, 2018.

[3] R. Lazarus, R. Frank, _A High-Speed Sorting Procedure_, Communications of the ACM, 3(1), 1960.

[4] Marcin Kasprowicz, _Złożoność obliczeniowa algorytmu_, algorytm.edu.pl, dostęp online.

[5] Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein, _Wprowadzenie do algorytmów_, PWN, 2022.
