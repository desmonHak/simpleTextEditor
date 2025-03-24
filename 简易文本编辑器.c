#include <stdio.h>
#include <stdlib.h>

struct stud_node {
    char name[100];
    struct stud_node *next;
};

struct stud_node *head, *tail;
FILE *fp = NULL;
char fname[100];
int open = 0, create = 0;

void mi_abrir(char fname[]); /* Abrir archivo */
struct stud_node *mi_crear(); /* Crear nuevo archivo */
void mi_mostrar(struct stud_node *head); /* Mostrar contenido del archivo */
struct stud_node *mi_insertar(struct stud_node *head); /* Insertar línea */
struct stud_node *mi_eliminar(struct stud_node *head); /* Eliminar línea */
struct stud_node *mi_copiar(struct stud_node *head); /* Copiar línea */
struct stud_node *mi_modificar(struct stud_node *head); /* Modificar línea */
void mi_buscar(struct stud_node *head); /* Buscar cadena */
struct stud_node *mi_reemplazar(struct stud_node *head); /* Reemplazar cadena */
void mi_guardar(struct stud_node *head, FILE *fp, char fname[]); /* Guardar y salir */
void mi_abandonar(FILE *fp, struct stud_node *head); /* Abandonar edición */
int mi_strcmp(const char *str1, const char *str2); /* Comparar cadenas */
int mi_strlen(char *p); /* Medir longitud de cadena */
char *mi_strcpy(char*str1, const char*str2); /* Copiar cadena */
char *mi_strcat(char*str1, char*str2); /* Concatenar cadenas */

int main()
{
    int choice;

    while (1) {
        printf("*******************************************************\n");
        printf("***  Elige: 1. Abrir archivo <nuevo o existente> 2. Mostrar contenido del archivo ***\n");
        printf("***         3. Insertar línea 4. Eliminar línea 5. Copiar línea 6. Modificar línea   ***\n");
        printf("***         7. Buscar cadena 8. Reemplazar cadena         ***\n");
        printf("***         9. Guardar y salir 0. Abandonar edición      ***\n");
        printf("*******************************************************\n");
        scanf("%d", &choice);
        
        while (open == 0 && choice != 1) {
            printf("El archivo no está abierto, por favor ingresa una opción válida: ");
            scanf("%d", &choice);
        }
        
        while (open == 1 && choice == 1) {
            printf("El archivo ya está abierto, por favor ingresa una opción válida: ");
            scanf("%d", &choice);
        }
        
        switch (choice) {
        case 1:
            printf("Por favor, elige: 1: Abrir archivo 2: Crear archivo nuevo :");
            scanf("%d", &choice);
            if (choice == 1) {
                printf("Por favor, ingresa el nombre del archivo <puede incluir unidad, ruta, no más de 19 caracteres> (solo se admiten caracteres en inglés):");
                scanf("%s", fname);
                mi_abrir(fname);
            } else {
                head = mi_crear(head);
            }
            break;
        case 2: mi_mostrar(head); break;
        case 3: head = mi_insertar(head); break;
        case 4: head = mi_eliminar(head); break;
        case 5: head = mi_copiar(head); break;
        case 6: head = mi_modificar(head); break;
        case 7: mi_buscar(head); break;
        case 8: head = mi_reemplazar(head); break;
        case 9: mi_guardar(head, fp, fname); break;
        case 0: mi_abandonar(fp, head); break;
        }
    }
    return 0;
}

/* Abrir archivo */
void mi_abrir(char fname[])
{
    FILE *fp;
    char q[100];
    int choice = 0;
    struct stud_node *p;

    while ((fp = fopen(fname, "r")) == NULL) {
        printf("Error al abrir el archivo, elige una opción:  1: Intentar de nuevo 2: Crear archivo nuevo\n");
        scanf("%d", &choice);
        if (choice == 2) {
            head = tail = mi_crear();
            break;
        } else {
            printf("Por favor, ingresa el nombre del archivo <puede incluir unidad, ruta, no más de 19 caracteres>: ");
            scanf("%s", fname);
        }
    }
    if (choice != 2) {
        printf("Archivo abierto con éxito\n");
        open = 1;
        /* Usar lista enlazada para guardar los datos del archivo */
        head = tail = NULL;
        while (!feof(fp)) { // Salir del bucle al llegar al final del archivo
            p = (struct stud_node*)malloc(sizeof(struct stud_node));
            p->next = NULL;
            fgets(q, 100, fp);
            mi_strcpy(p->name, q);
            if (head == NULL) {
                head = p;
            } else {
                tail->next = p;
            }
            tail = p;
        }
        tail->next = NULL;
    }
}

/* Crear archivo */
struct stud_node *mi_crear()
{
    char q[100], flag[20];

    printf("Por favor, ingresa el nombre y ubicación del archivo a crear:");
    scanf("%s", fname);
    while ((fp = fopen(fname, "w")) == NULL) {
        printf("Error al crear, por favor intenta de nuevo:\n");
        scanf("%s", fname);
    }
    printf("Archivo creado con éxito\n");
    head = tail = NULL;
    create = 1;
    open = 1;

    return head;
}

/* Mostrar contenido del archivo */
void mi_mostrar(struct stud_node *head)
{
    int i = 1, sum = 0;
    struct stud_node *p, *pre;

    for (pre = head; pre != NULL; pre = pre->next) { // Recorrer la lista y contar los nodos
        sum++;
    }
    p = (struct stud_node*)malloc(sizeof(struct stud_node));
    p = head;
    if (sum == 0 || head == NULL) {
        printf("Este archivo está vacío\n");
    } else if (create == 1) {
        for (i = 1; i <= sum; i++) {
            printf("%d: %s", i, p->name);
            p = p->next;
        }
    } else {
        for (i = 1; i < sum; i++) {
            printf("%d: %s", i, p->name);
            p = p->next;
        }
    }
    printf("\n");
}

/* Insertar línea */
struct stud_node *mi_insertar(struct stud_node *head)
{
    struct stud_node *pre, *now, *p, *newhead, *newtail;
    int l, m, i, k, sum = 0;
    char q[100];

    newhead = newtail = NULL;
    pre = now = head;
    for (pre = head; pre != NULL; pre = pre->next) { // Recorrer la lista y contar los nodos
        sum++;
    }
    pre = head;
    printf("Por favor, ingresa l (número de la línea) y m (número de líneas a insertar): ");
    scanf("%d%d", &l, &m);
    if (l <= 0) {
        printf("El número de línea no es válido, por favor intenta de nuevo\n");
        return head;
    }
    printf("Por favor, ingresa el contenido de las líneas a insertar:\n");
    scanf("\n");

    /* Crear una nueva lista con el contenido ingresado */
    for (i = 0; i < m; i++) {
        p = (struct stud_node*)malloc(sizeof(struct stud_node));
        p->next = NULL;
        gets(q);
        mi_strcat(q, "\n");
        mi_strcpy(p->name, q);
        if (newhead == NULL) {
            newhead = p;
        } else {
            newtail->next = p;
        }
        newtail = p;
    }
    if (head == NULL) { // Si la lista está vacía
        head = newhead;
    } else { // Si la lista no está vacía
        k = 1;
        while (now->next != NULL && k != l) { // Encontrar la posición, ahora apunta a la línea l
            k++;
            pre = now;
            now = pre->next;
        } // k <= l
        if (l == 1) { // Insertar al principio
            newtail->next = now;
            head = newhead;
        } else if (l > sum) { // Insertar al final
            newtail->next = NULL;
            now->next = newhead;
        } else { // Insertar en el medio
            newtail->next = now;
            pre->next = newhead;
        }
    }
    return head;
}
