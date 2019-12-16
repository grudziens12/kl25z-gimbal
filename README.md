* # KL25Z Gimbal Project
 * ### Opis projektu
  #### Celem projektu było stworzenie urządzenia do stabilizacji nagrywanego obrazu za pomocą akcelerometru *MMA8451Q* wbudowanego w płytkę rozwojową *FRDM-KL25Z*.
  #### Na podstawie wskazań akcelerometru komunikującego się z mikrokontrolerem za pomocą magistrali *I2C* procesor komunikuje się z zewnętrznymi sterownikami *ULN2003* dwóch silników krokowych *28BYJ-48–5V*, które swoją pracą utrzymują stabilną pozycję urządzenia nagrywającego  kompensując zewnętrzne ruchy ramienia osoby nagrywającej.
 * ### Analiza Problemu
 #### *MMA8451Q* to akcelerometr o dynamicznie wybieranych skalach ±2 g/±4 g/±8 g i 14-bitowej rozdielczości, obsługujący również wyjście 8-bitowe. Możliwa jest także filtracja danych zbieranych przez urządzenie. Układ posiada dwa programowalne piny przerwań, co pozwala wykorzystać energooszczędne rozwiązania. Komunikację zapewnia wbudowany interfejs *I2C*.
 #### *28BYJ-48–5V* to pięcioprzewodowy unipolarny silnik krokowy zintegrowany z przekładnią 64:1.
 #### *ULN2003* jest 7-kanałowym układem Darlingtona zamkniętym w obudowie *SO-16* wraz z dodatkowymi elementami, takimi jak diody zabezpieczające. Służy on m.in. do sterowania silnikami krokowymi.
 #### *I2C* to magistrala dwuprzewodowa pozwalająca łączyć wiele urządzeń. Jednym przewodem przesyłany jest zegar, a drugim dane. Każde urządzenie posiada własny adres umożliwiający jego identyfikację, a komunikacja oparta jest o hierarchię master-slave.
 #### Głównym problemem jest odpowiednia analiza danych z akcelerometru, aby właściwie wysterować sterowniki silników krokowych, korzystając z pinów GPIO płytki rozwojowej.
 * ### Plan Realizacji
 #### Cały układ będzie zasilany z baterii 9V i umieszczony w obudowie wykonanej za pomocą drukarki 3D. Wykorzystana zostanie płytka rozwojowa *FRDM-KL25Z*, dwa sterowniki *ULN2003*, dwa silniki krokowe *28BYJ-48–5V*. Zostanie również wykorzystany przełącznik umożliwiający włączanie i wyłączanie urządzenia. Ze względu na zasilanie bateryjne, oprogramowanie mikrokontrolera zostanie zoptymalizowane pod względem zużycia energii.
 * ### Schemat blokowy
 ![](Diagram.png&s=200)
