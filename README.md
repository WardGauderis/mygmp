# MyGMP - project telecom 2021
### Thomas Dooms & Ward Gauderis

---

## Elementen
Voor zowel de router als de client hebben we geprobeerd 
om dezelfde elementstructuur te hanteren. 

Bij de client bestaat dit uit:
- **ClientFilter**: dit element filtert binnenkomende datapakketten die de client wilt.
  
- **Client**: dit element bevat alle logica rond interface changes, queries beantwoorden en reports sturen.

- **ClientState**: dit is een gedeeld element (infobase) tussen de ClientFilter en Client element die de state bevat.
  
De router is ongeveer hetzelfde:
- **RouterFilter**: dit element stuurt de binnenkomende pakketten naar de overeenkomende interface(s).
- **Router**: dit element behandelt de reports en het versturen van group-specific en general queries.
- **RouterState**: dit is opnieuw een gedeeld element dat de lijst van groepen/interfaces bijhoudt.

Bijkomend hebben we ook enkele hulpelementen.
- **AlertEncap**: voegt de alert option toe aan een bestaand ip pakket.
- **FixIpDest**: Verandert de ip-dest door het group-address uit de igmp data.

Al deze elementen kunnen gevonden worden onder *elements/local/igmp*.

## Click scripts
Enkel in de scripts *library/client.click* en *library/router.click* werden het client- en routerelement aangepast
door in de static lookup table een entry te maken die IGMP-groupaddresses filtert en naar onze elementen doorstuurt.

## Test scripts
Om onze implementatie zelf te testen en om aan te tonen hebben 
we enkele *expect* scriptjes geschreven. Deze expect scriptjes 
laten toe verschillenden connecties te opnenen en handlers uit 
te voeren (leave/joins). Deze kunnen gevonden worden in de scripts 
folder. 

- **multiple.exp**: 
  dit scriptje test joins en leaves op beide interfaces van de 
  router. Op interface 1 gebeuren er 2 joins gevolgd door 1 leave. 
  Normaal moeten pakketten hier blijven aankomen.
  Op interface 2 gebeuren 2 joins en 2 leaves, waardoor pakketten 
  na een tijdje moeten stoppen.
  

- **duplicate.exp**: Dit is om te testen ofdat een router duplicate 
  joins en leaves correct behandelt.
  

- **join_leave.exp**: Dit is een eenvoudige test met 2 joins en 2 leaves, 
  de pakketten zouden moeten stoppen.


- **timeout.exp**: Dit is een test waarin een client disconnect 
  zonder te leaven om te testen of dit correct behandelt wordt.