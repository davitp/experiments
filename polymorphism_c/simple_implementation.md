Բազմաձևության իրականացում C լեզվով
=============================
  Ողջույն։ Եթե ծանոթ եք օբյեկտ կողմնորոշված ծրագրավորման (ՕԿԾ) գաղափարին, ապա, ըստ երևույթին, առընչվել եք նաև բազմաձևության([պոլիմորֆիզմ](http://en.wikipedia.org/wiki/Polymorphism_%28computer_science%29)) հետ։ 

  Չխորանալով սահմանումների և տերմինաբանության մեջ կարող ենք ասել, որ բազմաևությունը մեխանիզմ է, որն ապահովում է "Մեկ ինտերֆեյս, տարբեր իրականացումներ մեխանիզմը"։ Վերջինս C++երեն հնչում է այսպես.
>  Բազմաևությունը հնարավորություն է տալիս բազային դասի ցուցիչով կանչել ժառանգ դասերի համար վերասահմանված մեթոդները։ 
  
  Նման մեխանիզմ իրականացնելու համար ծրագրավորման լեզվի մշակողները տարբեր մոտեցումներ ունեն, սակայն այս գրառման մեջ կդիտարկվի դրանցից միայն մեկը՝ վիրտուալ մեթոդների աղուսյակով բազմաձևության կազմակերպումը։ 
  
  Նախ դիտարկենք օրինակ C++ լեզվով, ինչից հետո նույն օրինակը կիրականացնենք C-ով։ 
  
  Օրինակը հետևյալն է․
> Ունենք Animal դաս, որն օժտված է say() մեթոդով։ Մեթոդն էկրանին դուրս է բերում "Animal says Hello!" արտահայտությունը։  
> Animal դասն ունի երկու ժառանգ Tiger և Lion, որոնցում համապատասխանաբար վերասահմանված է say() մեթոդը։ 
> Խնդիրը կայանում է նրանում, որ պետք է Animal* տիպի ցուցիչով կարողանանք դիմել նաև Tiger և Lion դասերում վերասահմանված say() մեթոդին և էկրանին ստանանք ճիշտ հաղորդագրություն։ 

### Իրականացումը C++ով 
  
  ```c++
  // սահմանում ենք Animal դասը 
  class Animal {
  public:
  	// ուշադրություն դարձրեք, որ say մեթոդը սահմանված է, որպես
  	// վիրտուալ (virtual բանալի բառով)
	  virtual void say(){
		  cout << "Animal says hello!" << endl;	
	  }
  };
  
  // սահմանում ենք Tiger դասը ժառանգելով Animal դասից
  class Tiger : public Animal {
  public:
  	  // քանի որ բազային դասում say()-ը նշված է որպես virtual
  	  // վերասահմանելիս կարելի է virtual բառը բաց թողնել
	  virtual void say(){
		  cout << "Tiger says hello!" << endl;	
	  }
  };

  // սահմանում ենք Lion դասը ժառանգելով Animal դասից
  class Lion : public Animal {
  public:
	  virtual void say(){
		  cout << "Lion says hello!" << endl;	
	  }
  };

  ```
  
  Օրինակն իրականացված է։ Մնում է գրենք կիրառությունը։
  ```c++
  int main(){
    // սահմանենք 3 օբյեկտ բազային դասի ցուցիչով 
    
    // Animal օբյեկտ Animal* ցուցիչով
    Animal* animalPtr = new Animal();
    
    // Tiger օբյեկտ Animal* ցուցիչով
    Animal* tigerPtr = new Tiger();
    
    // Lion օբյեկտ Animal* ցուցիչով
    Animal* lionPtr = new Lion();
    
    /* say մեթոդի կանչ */
    animalPtr->say(); // Animal::say()
    tigerPtr->say(); // Tiger::say()
    lionPtr->say(); // Lion::say()
    
    /* կըտոր մը մաքրութիւն */
    delete lionPtr;
    delete tigerPtr;
    delete animalPtr;
    
    // ծրագրի հաջող ավարտ
    return 0;
}
  ```

  Եթե մեթոդները վերասահմանված չլինեին որպես virtual, ապա երեք անգամ կկանչվեր Animal::say() մեթոդը։ Համոզվելու համար բավական է հեռացնել virtual բառը։ 

  Իսկ ինչպե՞ս է այս ամենն աշխատում։ Ինչպե՞ս է թարգմանիչը հասկանում, որ դասի մեթոդը կանչել (չէ որ երեք դեպքում էլ դիմում ենք Animal* տիպի ցուցչի)։ 

  Մեխանիզմը հետևյալն է
  + ամեն դասի համար ստեղծվում է վիրտուալ մեթոդների աղուսյակ (vtable) 
  + աղուսյակը մասնավորապես կարող է իրականացված լինել որպես կառուցվածք (ստորեվ բերված օրինակում) կամ երկչափ զանգված 
  + derive.vtable.virtualFunction դաշտին վերագրվում է համապատասխան վերասահմանված ֆունկցիայի ցուցիչը
  + derived.virtualFunction() դիպի կանչը փոխարինվում է derived.vtable.virtualFunction()
  	


### Իրականացումը C-ով 

```c
// վիրտուալ մեթոդների աղուսյակի նկարագրություն Animal դասի համար
typedef struct animal_vtable_type {
    // say վիրտուալ մեթոդի ցուցիչի համար նախատեսված դաշտ
    void (*say)();
    
    
    // այստեղ կարող են լինել այլ դաշտեր եթե դասը պարունակում է 
    // մեկից ավել վիրտուալ մեթոդներ
} animal_vtable_t;

```

Համապատասխան տիպերի հայտարարում։ Այստեղ ինչ֊որ կերպ իրականացված է նաև ժառանգման մեխանիզմը։ 
```c
// Animal դասի անալոգ
typedef struct animal_type {
    // ուշադրություն դարձրեք այն բանին, 
    // դասը չի պարունակում say մեթոդի ցուցիչ
    // դրա փոխարեն վիրտուալ մեթոդների աղուսյակի օբյեկտ է
    animal_vtable_t vtable;
} animal_t;


// Tiger դասի անալոգ 
typedef struct tiger_type {
    // vtable աղուսյակի օբյեկտ
    animal_vtable_t vtable;
    // ժառանգման սիմուլյացիա
    animal_t base;
} tiger_t;

// Lion դասի անալոգ
typedef struct lion_type {
    // vtable աղուսյակի օբյեկտ
    animal_vtable_t vtable;
    // ժառանգման սիմուլյացիա
    animal_t base;
} lion_t;

// Հարկավոր է նկարագրել նաև վերոնշյալ «դասերի»
// լռությամբ կառուցիչ (default constructor) ֆունկցիաներ

// Animal-ի համար
void animal_constructor(animal_t* this);

// Tiger-ի համար
void tiger_constructor(tiger_t* this);

// Lion-ի համար
void lion_constructor(lion_t* this);

```

Նկարագրենք say() մեթոդի համապատասխան «վերասահմանումները»
```c
void animal_say(void);
void tiger_say(void);
void lion_say(void);
```

Դիտարկենք կիրառությունը, ինչից հետո կվերադառնանք նկարագրված ֆունկցիաների իրականացմանը։
```c
int main(void){
    // Animal օբյեկտի սահմանում
    animal_t animal;
    // սկզբնարժեքավորում (default constructor)
    animal_constructor(&animal);

    // Tiger օբյեկտի սահմանում
    tiger_t tiger;
    // սկզբնարժեքավորում (default constructor)
    tiger_constructor(&tiger);

    // Lion օբյեկտի սահմանում
    lion_t lion;
    // սկզբնարժեքավորում (default constructor)
    lion_constructor(&lion);

    // ստեղծենք երեք ցուցիչ Animal*-ի անալոգով,
    // որոնք ցույց են տալիս տարբեր տիպի օբյեկտների 
    // վրա
    animal_t* animalPtr = (animal_t*) &animal;
    animal_t* tigerPtr = (animal_t*) &tiger;
    animal_t* lionPtr = (animal_t*) &lion;

    /* կանչի օրինակներ */
    
    printf("Calling Anlimal's say()\n");
    // Animal::say() կանչ
    animalPtr->vtable.say();
    printf("\n");

    printf("Calling Tiger's say()\n");
    // Tiger::say() կանչ
    tigerPtr->vtable.say();
    printf("\n");

    printf("Calling Lion's say()\n");
    // Lion::say() կանչ
    lionPtr->vtable.say();
    printf("\n");

    // ծրագրի հաջող ավարտ
    return 0;
}
```

Ինչպես տեսանք երեք ցուցիչներն էլ բազային տիպի էին (Animal*), սակայն կանչի ժամանակ էկրանին դուրս բերվեցին ճիշտ հաղորդագրություններ։ 

Մնաց դիտարկենք վերը նկարագրված ֆունկցիաների իրականացումները։ 
```c
// Animal::say() 
void animal_say(void){
    printf("Animal says Hello! (Base)\n");
}

// Tiger::say() 
void tiger_say(void){
    printf("Tiger says Hello! (Derived 1)\n");
}

// Lion::say() 
void lion_say(void){
    printf("Lion says Hello! (Derived 2)\n");
}

/* կառուցիչների իրականացում */ 

// Animal() կառուցիչ 
void animal_constructor(animal_t* this){
    // վիրտուալ մեթոդների աղուսյակի
    // դաշտերը սկզմբնարժեքավերվում են 
    // ժառանգ դասերում վերասահմանված 
    // ֆունկցիաների ցուցիչներով
    this->vtable.say = animal_say;
}

// Tiger() կառուցիչ 
void tiger_constructor(tiger_t* this){
    // աղուսյակի սկզբնարժեքավորում
    // tiger_say ցուցիչով
    this->vtable.say = tiger_say;
    
    // «բազային դասի» սկզբնարժեքավորում
    animal_constructor(&(this->base));
}

// Lion() կառուցիչ 
void lion_constructor(lion_t* this){
    // աղուսյակի սկզբնարժեքավորում
    // lion_say ցուցիչով
    this->vtable.say = lion_say;
    
    // «բազային դասի» սկզբնարժեքավորում
    animal_constructor(&(this->base));
}
```

Օրինակը թարգմանվել է GCC 4.9֊ով, հետևաբար որոշ մեքենաների վրա այլ կարող են առաջ գալ տարբեր թարգմանության սխալներ, որոնք շատ հեշտ կարելի է ուղղել։ 

Պոլիմորֆիզմի C-ական իրականացում կարելի է տեսնել բազմաթիվ հայտնի գրադարաններում, ինչպիսին է օրինակ GTK-ն։

Այսքանը։
