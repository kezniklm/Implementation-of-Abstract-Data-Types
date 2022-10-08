/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) 
{
	
	list->firstElement = NULL; 
	list->activeElement = NULL; //inicializacia zoznamu aby sa predislo nedefinovaným hodnotám a nedefinovanemu chovaniu
	list->lastElement = NULL; 
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) 
{
	while(list->firstElement != NULL)
	{
		DLLElementPtr temp_element = list->firstElement->nextElement;
		free(list->firstElement);	
		list->firstElement = temp_element;
	}

	list->activeElement = NULL;		
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) //fix
{
	struct DLLElement *new_element = NULL;
	new_element = (struct DLLElement *) malloc(sizeof(struct DLLElement));
	if(new_element == NULL)
	{
		DLL_Error();
		return;
	}
	else 
	{
		//Pokial v zozname nie je ziadny prvok -> zoznam je prazdny a vlozeny prvok je prvy a zaroven aj poslednym prvkom
		if((list->firstElement == NULL) && (list->lastElement == NULL) )
		{
			list->firstElement = new_element;
			list->lastElement = new_element;
			new_element->nextElement = NULL;
			new_element->previousElement = NULL;
			new_element->data = data;
		}
		//Pokial v zozname su nejake prvky, novo vlozeny prvok sa stane prvym prvkom 
		else
		{
			DLLElementPtr temp_element = list->firstElement;
			list->firstElement = new_element;
			new_element->nextElement = temp_element;
			new_element->previousElement = NULL;
			new_element->data = data;
			temp_element->previousElement = new_element;
		}
	}
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) //fix
{
	struct DLLElement *new_element = NULL;
	new_element = (struct DLLElement *) malloc(sizeof(struct DLLElement));
	if(new_element == NULL)
	{
		DLL_Error();
		return;
	}
	else 
	{
		//Pokial v zozname nie je ziadny prvok -> zoznam je prazdny a vlozeny prvok je prvy a zaroven aj poslednym prvkom
		if((list->firstElement == NULL) && (list->lastElement == NULL) )
		{
			list->firstElement = new_element;
			list->lastElement = new_element;
			new_element->nextElement = NULL;
			new_element->previousElement = NULL;
			new_element->data = data;
		}
		//Pokial v zozname su nejake prvky, novo vlozeny prvok sa stane poslednym prvkom 
		else
		{
			DLLElementPtr temp_element = list->lastElement;
			list->lastElement = new_element;
			new_element->nextElement = NULL;
			new_element->previousElement = temp_element;
			new_element->data = data;
			temp_element->nextElement = new_element;
		}
	}		
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) 
{
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) 
{
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) 
{
	if ((list->firstElement == NULL) || (list->lastElement == NULL))
	{
		DLL_Error();
		return;
	}
	else 
	{
		*dataPtr = list->firstElement->data;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) 
{
	if ((list->firstElement == NULL) || (list->lastElement == NULL))
	{
		DLL_Error();
		return;
	}
	else 
	{
		*dataPtr = list->lastElement->data;
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) 
{
   	if (list->firstElement != NULL) 
   	{
       	//Pokiaľ bol prvý prvok aktivny, aktivita sa stráca
    	if (list->firstElement == list->activeElement) 
		{
        	list->activeElement = NULL;
    	}
    	DLLElementPtr trash_element = list->firstElement;

		//Ošetrenie situácie, kedy sa v zozname nacházda iba jeden prvok
    	if (trash_element == list->lastElement) 		
		{
        	list->lastElement = NULL;
			list->firstElement = trash_element->nextElement;
			free(trash_element); 
    	}

		//Ošetrenie situácie, kedy sa v zozname nacházda viacej prvkov
    	else 
		{
    	    trash_element->nextElement->previousElement = NULL;
			list->firstElement = list->firstElement->nextElement;
			free(trash_element); 
    	}
	}  
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) 
{
    if (list->firstElement != NULL) 
	{
       	//Pokiaľ bol prvý prvok aktivny, aktivita sa stráca
   	 	if (list->lastElement == list->activeElement) 
		{
        	list->activeElement = NULL;
    	}

    	DLLElementPtr trash_element = list->lastElement;
    	//Ošetrenie situácie, kedy sa v zozname nacházda iba jeden prvok
    	if (list->firstElement == trash_element) 
		{
        	list->firstElement = NULL;
			list->lastElement = trash_element->previousElement;
    		free(trash_element);
    	}

    	//Ošetrenie situácie, kedy sa v zozname nacházda viacej prvkov
    	else 
		{
        	trash_element->previousElement->nextElement = NULL;
			list->lastElement = list->lastElement->previousElement;
    		free(trash_element);
    	}
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) 
{
    if (list->activeElement != NULL)  
	{
		if(list->activeElement != list->lastElement)
		{
    		DLLElementPtr trash_element = list->activeElement->nextElement;
    		list->activeElement->nextElement = trash_element->nextElement;

			//Ošetrenie situácie, kedy je aktívny prvok predposledný a teda vymazávaný prvok je posledný
    		if (list->lastElement == trash_element) 
			{
        		list->lastElement = list->activeElement;
				free(trash_element);
    		}

			//Ošetrenie situácie, kedy nie je aktívny prvok predposledný
    		else 
			{
        		trash_element->nextElement->previousElement = list->activeElement;
				free(trash_element);
    		}
		}
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) 
{
    if (list->activeElement != NULL) 
	{
		if( list->activeElement != list->firstElement) 
		{
    		DLLElementPtr trash_element = list->activeElement->previousElement;
    		list->activeElement->previousElement = trash_element->previousElement;
			//Ošetrenie situácie, kedy je aktívny prvok druhý a teda vymazávaný prvok je prvý

    		if (trash_element == list->firstElement) 
			{
        		list->firstElement = list->activeElement;
				free(trash_element);
    		}
			
			//Ošetrenie situácie, kedy nie je aktívny prvok druhý
    		else 
			{
        		trash_element->previousElement->nextElement = list->activeElement;
				free(trash_element);
    		}
		}
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) 
{
  	if (list->activeElement != NULL) 
  	{
    	struct DLLElement *new_element = NULL;
		new_element = (struct DLLElement *) malloc(sizeof(struct DLLElement));
		if(new_element == NULL)
		{
			DLL_Error();
			return;
		}

    	new_element->data = data;
    	new_element->previousElement = list->activeElement;
    	new_element->nextElement = list->activeElement->nextElement;
    	list->activeElement->nextElement = new_element;

    	//Ošetrenie situácie, kedy pridávame nový prvok na koniec
    	if (list->activeElement == list->lastElement) 
		{
        	list->lastElement = new_element;
    	}

    	//Ošetrenie situácie, kedy nepridávame nový prvok na koniec
    	if (new_element->nextElement != NULL) 
		{
        	new_element->nextElement->previousElement = new_element;
    	}	
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) 
{
	if (list->activeElement != NULL) 
  	{
    	struct DLLElement *new_element = NULL;
		new_element = (struct DLLElement *) malloc(sizeof(struct DLLElement));
		if(new_element == NULL)
		{
			DLL_Error();
			return;
		}

    	new_element->nextElement = list->activeElement;
    	new_element->previousElement = list->activeElement->previousElement;
		new_element->data = data;
    	list->activeElement->previousElement = new_element;

		//Ošetrenie situácie, kedy pridávame nový prvok na začiatok
    	if (list->activeElement == list->firstElement) 
		{
        list->firstElement = new_element;
    	}

		//Ošetrenie situácie, kedy nepridávame nový prvok na začiatok
    	if (new_element->previousElement != NULL) 
		{
        new_element->previousElement->nextElement = new_element;
    	}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) 
{
	if(list->activeElement == NULL)
	{
		DLL_Error ();
		return;
	}
	else
	{
		*dataPtr = list->activeElement->data;
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) 
{
	if(list->activeElement != NULL)
	{
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) 
{
	if(list->activeElement != NULL)
	{
		list->activeElement = list->activeElement->nextElement;
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) 
{
	if(list->activeElement != NULL)
	{
		list->activeElement = list->activeElement->previousElement;
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) 
{
	if(list->activeElement != NULL)
	{
		return TRUE;
	}
	else return FALSE;
}

/* Konec c206.c */
