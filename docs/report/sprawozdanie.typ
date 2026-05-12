#import "generated-data.typ": generated-cocktail-singlelist-limit, generated-task-summary

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

#let scan-row(size) = {
  for row in generated-cocktail-singlelist-limit {
    if row.size == size {
      return row
    }
  }
  panic("Missing generated scan row for size " + str(size))
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
  // #v(1.2cm)
  // #text(size: 14pt, weight: "bold")[Analiza algorytmów sortowania i wpływu struktury danych na czas wykonania]
  #v(1.4cm)
  #text(size: 12pt)[Mykhailo Tsonyev]
  #text(size: 12pt)[nr albumu 284497]
  #v(0.6cm)
  #text(size: 11pt)[Prowadzący kurs: Damian Mroziński]
  #linebreak()
  #text(size: 11pt)[Termin zajęć: TN wtorek 9:15]
  #linebreak()
  #text(size: 11pt)[Termin oddania zadania: 12.05.2026]
]

#pagebreak()

= Wstęp

Celem projektu było zaimplementowanie programu sortującego dane rosnąco oraz przeprowadzenie analizy czasu działania wybranych algorytmów sortowania.

W zakresie wymaganym na ocenę `3.0` zaimplementowano algorytmy `CocktailSort`, `MergeSort` i `InsertionSort` oraz dwie własne struktury liniowe: `DynamicArray` i `SinglyLinkedList`. Program oprócz samego sortowania sprawdza poprawność wyniku i zapisuje wyniki pomiarów do pliku CSV.

Eksperyment obejmuje trzy główne badania: `Badanie A`, `Badanie B` i `Badanie C`. Ich szczegółowy układ oraz sposób interpretacji wyników zostały opisane w rozdziale `Metodyka badań`.

Program uruchamiano na komputerze `MacBook Air M1` z `8 GB RAM`. Główne wyniki pochodzą z pliku `results/research_data_without_cocktail_singlelist.csv`, a dodatkowy test dla `CocktailSort` i `SinglyLinkedList` z pliku `results/cocktail_singlelist_limit_status.csv`.


= Metodyka badań

Eksperyment został podzielony na trzy części: `Badanie A`, `Badanie B` i `Badanie C`. Każdy benchmark wykonano 50 razy.

W pliku CSV zapisano surowy czas każdej iteracji. Wartości `min_us`, `avg_us` i `max_us` pojawiają się tylko w ostatnim wierszu danego bloku 50 iteracji.

Podział benchmarków pomiędzy główne badania był następujący:

- `Badanie A` obejmuje 20 benchmarków:
  `CocktailSort` dla `DynamicArray`, `MergeSort` dla `DynamicArray` i `SinglyLinkedList` oraz `InsertionSort` dla `DynamicArray` i `SinglyLinkedList`, zawsze dla rozmiarów `5000`, `10000`, `25000` i `50000`.
- `Badanie B` obejmuje 8 benchmarków:
  `MergeSort` dla `DynamicArray` i `SinglyLinkedList`, a dla każdej struktury rozkłady `random`, `ascending`, `ascending50Per` i `descending`.
- `Badanie C` obejmuje 3 benchmarki:
  `MergeSort` dla `DynamicArray` i rozmiaru `25000` przy typach `int`, `float` i `unsigned int`.

`CocktailSort` dla `SinglyLinkedList` sprawdzono osobno poza głównym zestawem badań. Dla tej kombinacji wykonano pojedyncze uruchomienia dla kilku mniejszych rozmiarów, aby pokazać, jak szybko rośnie czas działania.


= Opis implementacji

Implementacja używa szablonów, dzięki czemu te same algorytmy mogą pracować na różnych typach danych bez powielania kodu. Wspólny interfejs struktur zapewnia klasa `IStructure<T>`, która udostępnia metody `size()`, `operator[]`, `pushBack()` oraz `swap()`. Tam, gdzie to miało sens, wykorzystano wspólny interfejs. Tam, gdzie lista wymagała innego podejścia, przygotowano osobną implementację pracującą bezpośrednio na węzłach [2][5].

== Struktura DynamicArray

`DynamicArray<T>` przechowuje dane w ręcznie zarządzanym buforze pamięci `data`, a dodatkowo zapisuje `currentSize` oraz `capacity`. Struktura udostępnia dostęp indeksowy przez `operator[]`, zwraca bieżącą liczbę elementów przez `size()`, pozwala dopisać element na koniec przez `pushBack()` i zamieniać miejscami dwa elementy przez `swap(i, j)`. Dodatkowo posiada konstruktor kopiujący, operator przypisania oraz destruktor, które odpowiadają za poprawne zarządzanie pamięcią.

Z punktu widzenia algorytmów najważniejsze jest to, że `DynamicArray` oferuje szybki dostęp do elementu o zadanym indeksie. Dzięki temu klasyczne wersje `CocktailSort`, `MergeSort` i `InsertionSort` działają tutaj bez dodatkowych utrudnień.

== Struktura SinglyLinkedList

`SinglyLinkedList<T>` przechowuje elementy w węzłach `Node`, z których każdy zawiera wartość `value` i wskaźnik `next` na kolejny węzeł. Struktura posiada wskaźniki `head` i `tail`, dzięki czemu zna początek i koniec listy, a także pole `currentSize`, które przechowuje liczbę elementów. Publiczne operacje obejmują `pushBack()`, `size()`, `operator[]` oraz `swap(i, j)`. W części prywatnej znajdują się też metody pomocnicze `clear()`, `copyFrom()` i `nodeAt(index)`.

W praktyce ta struktura jest używana inaczej niż `DynamicArray`. `MergeSort` i `InsertionSort` dla `SinglyLinkedList` pracują bezpośrednio na węzłach i wskaźnikach `next`. Dzięki temu lista jest sortowana zgodnie ze swoją naturą, bez sztucznego udawania tablicy [2][5].

== Algorytm CocktailSort

`CocktailSort` wykonuje dwa przejścia w obrębie aktualnego zakresu danych: najpierw od lewej do prawej, przesuwając większe elementy ku końcowi, a następnie od prawej do lewej, przesuwając mniejsze elementy ku początkowi. Dla `DynamicArray` algorytm korzysta bezpośrednio z `operator[]` i `swap()`.

Dla `SinglyLinkedList` nie przygotowano osobnej implementacji tego algorytmu. `CocktailSort` wymaga przejść w obie strony, a lista jednokierunkowa nie pozwala wygodnie wracać do poprzednich elementów. Z tego powodu w obecnej wersji programu dla `SinglyLinkedList` pozostawiono tylko fallback przez `operator[]` z interfejsu `IStructure<T>`. Taki wariant jest poprawny, ale bardzo wolny. Sam `CocktailSort` ma złożoność `O(n^2)`, a pojedynczy dostęp do elementu listy przez indeks kosztuje `O(n)`, więc w praktyce ta kombinacja zbliża się do `O(n^3)` [3][5].

== Algorytm MergeSort

`MergeSort` działa według schematu dziel i zwyciężaj. Dla `DynamicArray` algorytm rekurencyjnie dzieli zakres indeksów na dwie połowy, sortuje każdą z nich, a następnie scala obie uporządkowane części. W czasie scalania używany jest jeden dodatkowy bufor pomocniczy, czyli tymczasowa tablica `buffer`, do której przepisywane są elementy w trakcie łączenia dwóch posortowanych fragmentów. Bufor ten jest alokowany tylko raz dla całego sortowania, a nie przy każdym poziomie rekurencji [1][5].

Dla `SinglyLinkedList` zastosowano osobną implementację opartą na wskaźnikach. Lista jest dzielona na dwie połowy metodą szybkiego i wolnego wskaźnika, a następnie scalana przez przepinanie pól `next`. Implementacja zwraca przy tym od razu nowy `head` i `tail`, dzięki czemu nie trzeba na końcu ponownie przechodzić całej listy. Taki wariant dobrze pasuje do listy jednokierunkowej i pozwala utrzymać oczekiwaną złożoność `O(n log n)` [1][2][5].

== Algorytm InsertionSort

`InsertionSort` przetwarza dane od lewej do prawej. W każdej iteracji pobiera bieżący element, porównuje go z elementami poprzedzającymi i przesuwa większe wartości o jedno miejsce w prawo, aż znajdzie pozycję, w którą można wstawić rozpatrywany element. Dla `DynamicArray` operacja ta jest realizowana bezpośrednio na indeksach tablicy [1][5].

Dla `SinglyLinkedList` zastosowano osobną wersję algorytmu pracującą bezpośrednio na węzłach. Kolejne elementy są wyjmowane z listy wejściowej i wstawiane we właściwe miejsce nowej uporządkowanej części przez zmianę wskaźników `next`. Po zakończeniu aktualizowane są również `head` i `tail` listy. Dzięki temu implementacja nie korzysta z `operator[]` i lepiej odpowiada tej strukturze danych [2][5].


= Wyniki badań

== Badanie A

Badanie A sprawdza wpływ liczebności zbioru na czas działania algorytmów. W tej części wykonano 20 benchmarków. Dla `DynamicArray` porównano `CocktailSort`, `MergeSort` i `InsertionSort`, a dla `SinglyLinkedList` porównano `MergeSort` i `InsertionSort`. `CocktailSort` dla listy został pokazany osobno w dodatkowym teście, ponieważ w uczciwej wersji przez `operator[]` bardzo szybko staje się niepraktyczny.

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
      [MergeSort], [5000], [#summary-row(5).min-us], [#summary-row(5).avg-us], [#summary-row(5).max-us],
      [MergeSort], [10000], [#summary-row(6).min-us], [#summary-row(6).avg-us], [#summary-row(6).max-us],
      [MergeSort], [25000], [#summary-row(7).min-us], [#summary-row(7).avg-us], [#summary-row(7).max-us],
      [MergeSort], [50000], [#summary-row(8).min-us], [#summary-row(8).avg-us], [#summary-row(8).max-us],
      [InsertionSort], [5000], [#summary-row(13).min-us], [#summary-row(13).avg-us], [#summary-row(13).max-us],
      [InsertionSort], [10000], [#summary-row(14).min-us], [#summary-row(14).avg-us], [#summary-row(14).max-us],
      [InsertionSort], [25000], [#summary-row(15).min-us], [#summary-row(15).avg-us], [#summary-row(15).max-us],
      [InsertionSort], [50000], [#summary-row(16).min-us], [#summary-row(16).avg-us], [#summary-row(16).max-us],
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
  caption: [Czas sortowania w zależności od liczebności zbioru dla struktury `SinglyLinkedList`.],
)

Przykładowo dla `DynamicArray` i rozmiaru `50000` średni czas `MergeSort` wyniósł #fmt-avg(summary-row(8).avg-us), podczas gdy `CocktailSort` osiągnął #fmt-avg(summary-row(4).avg-us), a `InsertionSort` #fmt-avg(summary-row(16).avg-us). Różnica jest więc bardzo duża.

== Dodatkowy test: CocktailSort dla SinglyLinkedList

Osobno wykonano pojedyncze uruchomienia `CocktailSort` dla `SinglyLinkedList` przy mniejszych rozmiarach danych. Test ten nie należał do głównego zestawu benchmarków. Jego celem było pokazanie, jak szybko ta kombinacja staje się zbyt wolna.

#figure(
  image("/results/plots/cocktail_singlelist_limit.png", width: 100%),
  caption: [Pojedyncze uruchomienia `CocktailSort` dla `SinglyLinkedList`.],
)

#figure(
  kind: table,
  align(center)[
    #table(
      columns: 3,
      stroke: 0.6pt,
      [Rozmiar], [Czas [us]], [Czas wykonania [s]],
      [100], [#scan-row(100).avg-us], [#scan-row(100).wall-time-sec],
      [200], [#scan-row(200).avg-us], [#scan-row(200).wall-time-sec],
      [500], [#scan-row(500).avg-us], [#scan-row(500).wall-time-sec],
      [1000], [#scan-row(1000).avg-us], [#scan-row(1000).wall-time-sec],
      [2000], [#scan-row(2000).avg-us], [#scan-row(2000).wall-time-sec],
      [3000], [#scan-row(3000).avg-us], [#scan-row(3000).wall-time-sec],
      [4000], [#scan-row(4000).avg-us], [#scan-row(4000).wall-time-sec],
      [5000], [#scan-row(5000).avg-us], [#scan-row(5000).wall-time-sec],
    )
  ],
  caption: [Pojedyncze czasy dla `CocktailSort` i `SinglyLinkedList`.],
)

Już przy `5000` elementach pojedyncze sortowanie trwało #fmt-avg(scan-row(5000).avg-us), czyli około #scan-row(5000).wall-time-sec s. To dobrze pokazuje, dlaczego tej kombinacji nie włączono do głównego zestawu badań.

== Badanie B

Badanie B dotyczy wpływu rozkładu danych wejściowych. W tej części wykonano 8 benchmarków: po cztery dla `DynamicArray` i `SinglyLinkedList`. Każdy benchmark używał `MergeSort`, a zmieniano jedynie rozkład danych wejściowych.

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
      [DynamicArray], [random], [#summary-row(21).min-us], [#summary-row(21).avg-us], [#summary-row(21).max-us],
      [DynamicArray], [ascending], [#summary-row(22).min-us], [#summary-row(22).avg-us], [#summary-row(22).max-us],
      [DynamicArray], [ascending50Per], [#summary-row(23).min-us], [#summary-row(23).avg-us], [#summary-row(23).max-us],
      [DynamicArray], [descending], [#summary-row(24).min-us], [#summary-row(24).avg-us], [#summary-row(24).max-us],
      [SinglyLinkedList], [random], [#summary-row(25).min-us], [#summary-row(25).avg-us], [#summary-row(25).max-us],
      [SinglyLinkedList], [ascending], [#summary-row(26).min-us], [#summary-row(26).avg-us], [#summary-row(26).max-us],
      [SinglyLinkedList],
      [ascending50Per],
      [#summary-row(27).min-us],
      [#summary-row(27).avg-us],
      [#summary-row(27).max-us],

      [SinglyLinkedList], [descending], [#summary-row(28).min-us], [#summary-row(28).avg-us], [#summary-row(28).max-us],
    )
  ],
  caption: [Czas sortowania w zależności od początkowego rozkładu elementów.],
)

Dla `DynamicArray` średni czas dla rozkładu `random` wyniósł #fmt-avg(summary-row(21).avg-us), dla `ascending` #fmt-avg(summary-row(22).avg-us), dla `ascending50Per` #fmt-avg(summary-row(23).avg-us), a dla `descending` #fmt-avg(summary-row(24).avg-us). Dla `SinglyLinkedList` najwolniejszy był ponownie wariant `random`, a najszybszy `descending`.

== Badanie C

Badanie C analizuje wpływ typu danych przy zachowaniu tego samego algorytmu, tej samej struktury oraz tego samego rozmiaru wejścia. W tej części wykonano 3 benchmarki: dla `int`, `float` i `unsigned int`.

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
      [int], [#summary-row(29).min-us], [#summary-row(29).avg-us], [#summary-row(29).max-us],
      [float], [#summary-row(30).min-us], [#summary-row(30).avg-us], [#summary-row(30).max-us],
      [unsigned int], [#summary-row(31).min-us], [#summary-row(31).avg-us], [#summary-row(31).max-us],
    )
  ],
  caption: [Czas sortowania w zależności od typu danych.],
)

Średnie czasy wyniosły odpowiednio:

- `int`: #fmt-avg(summary-row(29).avg-us)
- `float`: #fmt-avg(summary-row(30).avg-us)
- `unsigned int`: #fmt-avg(summary-row(31).avg-us)

= Analiza wyników

Wyniki pokazują wyraźną różnicę między `MergeSort` a pozostałymi algorytmami. Wraz ze wzrostem rozmiaru danych czas `MergeSort` rośnie znacznie wolniej niż czas `CocktailSort` i `InsertionSort`.

== Badanie A

W badaniu A najwyraźniej widać wpływ rozmiaru danych. Wraz ze wzrostem liczby elementów przewaga `MergeSort` staje się coraz większa. Dla `DynamicArray` przy `50000` elementów średni czas `MergeSort` wyniósł #fmt-avg(summary-row(8).avg-us), podczas gdy `InsertionSort` osiągnął #fmt-avg(summary-row(16).avg-us), a `CocktailSort` #fmt-avg(summary-row(4).avg-us). Dla `SinglyLinkedList` `MergeSort` również pozostaje wyraźnie szybszy od `InsertionSort`.

Dodatkowy test `CocktailSort` dla `SinglyLinkedList` potwierdza, że ta kombinacja nie nadaje się do większych rozmiarów danych. Już przy `5000` elementach pojedyncze uruchomienie trwało około #scan-row(5000).wall-time-sec s, dlatego dalsze zwiększanie rozmiaru wejścia przestaje być praktyczne.

== Badanie B

W badaniu B zmiana rozkładu danych wpływa na wynik, ale nie zmienia ogólnej charakterystyki `MergeSort`. Różnice między `random`, `ascending`, `ascending50Per` i `descending` są znacznie mniejsze niż różnice obserwowane wcześniej między algorytmami. W obu strukturach najwolniejszy okazał się wariant `random`, natomiast najkrótsze czasy uzyskano dla `ascending` i `descending`.

== Badanie C

Badanie C pokazuje wpływ typu danych przy stałym algorytmie, strukturze i rozmiarze wejścia. Najwyższy średni czas uzyskano dla typu `float`, natomiast wyniki dla `int` i `unsigned int` są do siebie zbliżone. Różnice te pozostają jednak wyraźnie mniejsze niż różnice wynikające ze zmiany algorytmu w badaniu A.

= Wnioski

Przeprowadzone pomiary potwierdzają, że przy większych danych najlepsze wyniki uzyskuje `MergeSort`. W badaniu A jego czasy były wielokrotnie niższe niż czasy `CocktailSort` i `InsertionSort`, szczególnie dla `DynamicArray` przy `25000` i `50000` elementów.

Istotne znaczenie miało również dopasowanie algorytmu do struktury danych. `MergeSort` i `InsertionSort` można było zaimplementować dla `SinglyLinkedList` w sposób zgodny z naturą listy, czyli przez pracę na węzłach i wskaźnikach `next`. `CocktailSort` nie jest pod tym względem równie korzystny. Wersja oparta na `operator[]` działa poprawnie, ale bardzo szybko staje się zbyt wolna, co potwierdził dodatkowy test.

Wpływ rozkładu danych wejściowych oraz prostego typu liczbowego jest zauważalny, ale mniejszy niż wpływ samego algorytmu. W badanym zakresie największe znaczenie dla czasu wykonania miało to, czy zastosowano `MergeSort`, czy algorytm o zachowaniu kwadratowym.

= Literatura

[1] Maciej M. Sysło, _Algorytmy_, Helion, 2016.

[2] L. Banachowski, K. Diks, W. Rytter, _Algorytmy i struktury danych_, Wydawnictwo Naukowe PWN, Warszawa, 2018.

[3] R. Lazarus, R. Frank, _A High-Speed Sorting Procedure_, Communications of the ACM, 3(1), 1960.

[4] Marcin Kasprowicz, _Złożoność obliczeniowa algorytmu_, algorytm.edu.pl, dostęp online.

[5] Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein, _Wprowadzenie do algorytmów_, PWN, 2022.
