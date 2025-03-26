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

void my_abrir(char fname[]); /* Abrir archivo */
struct stud_node *my_crear(); /* Crear nuevo archivo */
void my_mostrar(struct stud_node *head); /* Mostrar contenido del archivo */
struct stud_node *my_insertar(struct stud_node *head); /* Insertar línea */
struct stud_node *my_eliminar(struct stud_node *head); /* Eliminar línea */
struct stud_node *my_copiar(struct stud_node *head); /* Copiar línea */
struct stud_node *my_modificar(struct stud_node *head); /* Modificar línea */
void my_buscar(struct stud_node *head); /* Buscar cadena */
struct stud_node *my_reemplazar(struct stud_node *head); /* Reemplazar cadena */
void my_guardar(struct stud_node *head, FILE *fp, char fname[]); /* Guardar y salir */
void my_abandonar(FILE *fp, struct stud_node *head); /* Abandonar edición */
int my_strcmp(const char *str1, const char *str2); /* Comparar cadenas */
int my_strlen(char *p); /* Medir longitud de cadena */
char *my_strcpy(char*str1, const char*str2); /* Copiar cadena */
char *my_strcat(char*str1, char*str2); /* Concatenar cadenas */

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
                my_abrir(fname);
            } else {
                head = my_crear(head);
            }
            break;
        case 2: my_mostrar(head); break;
        case 3: head = my_insertar(head); break;
        case 4: head = my_eliminar(head); break;
        case 5: head = my_copiar(head); break;
        case 6: head = my_modificar(head); break;
        case 7: my_buscar(head); break;
        case 8: head = my_reemplazar(head); break;
        case 9: my_guardar(head, fp, fname); break;
        case 0: my_abandonar(fp, head); break;
        }
    }
    return 0;
}

/* Abrir archivo */
void my_abrir(char fname[])
{
    FILE *fp;
    char q[100];
    int choice = 0;
    struct stud_node *p;

    while ((fp = fopen(fname, "r")) == NULL) {
        printf("Error al abrir el archivo, elige una opción:  1: Intentar de nuevo 2: Crear archivo nuevo\n");
        scanf("%d", &choice);
        if (choice == 2) {
            head = tail = my_crear();
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
            my_strcpy(p->name, q);
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
struct stud_node *my_crear()
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
void my_mostrar(struct stud_node *head)
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
struct stud_node *my_insertar(struct stud_node *head)
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
        my_strcat(q, "\n");
        my_strcpy(p->name, q);
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

/*删除行*/
struct stud_node *my_delete(struct stud_node *head)
{
	int l, m, i, k,sum=0;
	struct stud_node *pre, *now;

	for(pre=head;pre!=NULL;pre=pre->next){//遍历链表，sum计算链表有多少个结点
		sum++;
	}
	if(sum==0||head==NULL){
		printf("此文本为空，无可修改行\n");
		return head;
	}
	printf("从第l行起删除m行，请输入l m:");
	scanf("%d%d", &l, &m);
	while(l<=0||l>sum){
		printf("删除的行号不对,请重新输选择\n");
		return head;
	}
	if (head != NULL && l == 1) {//被删除的为表头结点
		for (i = 0; i < m; i++) {
			now = head;
			head = head->next;
			free(now);
		}
		if (head == NULL) {
			printf("数据已全部删除\n");
		}
	}
	else {//被删除的为非表头结点  l!=1
		pre = head;
		now = pre->next;
		k = 2;
		while (now->next != NULL && k != l) {//找位置,now指向第k行
			k++;
			pre = now;
			now = pre->next;
		}//k==l
		for (i = 0; i < m && now != NULL; i++) {
			pre->next = now->next;
			free(now);
			now=pre->next;
		}
	}
	return head;
}
/*拷贝行*/
struct stud_node *my_copy(struct stud_node *head)
{
	int l, m, k, i, j, sum=0;//i用来找行，j用于循环计算
	struct stud_node *pre, *now, *p, *newhead, *newtail;

	for(pre=head;pre!=NULL;pre=pre->next){//遍历链表，sum计算链表有多少个结点
		sum++;
	}
	if(sum==0||head==NULL){
		printf("此文本为空，无拷贝数据\n");
		return head;
	}
	printf("把第l行开始的m行插在原k行之前，请输入l m k:");
	scanf("%d%d%d",&l,&m,&k);
	while(l<=0||k<=0){
		printf("选择的行号不对,请重新输选择\n");
		return head;
	}
	pre = now = head;
	newhead = newtail = NULL;
	i = 1;
	while (now->next !=NULL && i != l) {//找第l行
		i++;
		pre = now;
		now = pre->next;//now指向第l行，pre指向第l-1行
	}//i==l
	for (j = 0; j < m; j++) {//把要拷贝的行建成新的链表
		p = (struct stud_node*)malloc(sizeof(struct stud_node));
		my_strcpy(p->name, now->name);
		pre = now;
		now = pre->next;
		if (newhead == NULL) {
			newhead = p;
		}
		else {
			newtail->next = p;
		}
		newtail = p;
		}
	pre = now = head;
	i = 1;
	while (now->next != NULL && i != k) {//找第k行
		i++;
		pre = now;
		now = pre->next;
	}//i==k
	if (k == 1) {//插在第一行前面
		newtail->next = now;
		head = newhead;
	}
	else if(k>sum){//插在尾部
		printf("已超过最大行号，默认插在最后一行\n");
		my_strcat(now->name,"\n");
		newtail->next = NULL;
		now->next = newhead;
	}
	else {//插在中间
		newtail->next = now;
		pre->next = newhead;
	}
	return head;
}
/*修改行*/
struct stud_node *my_mix(struct stud_node *head)
{
	struct stud_node *pre, *now;
	int i,k,sum=0;

	for(pre=head;pre!=NULL;pre=pre->next){//遍历链表，sum计算链表有多少个结点
		sum++;
	}
	if(sum==0||head==NULL){
		printf("此文本为空，无可修改行\n");
		return head;
	}
	printf("请输入待修改的行号：");
	scanf("%d", &k);
	if(k<=0){
		printf("修改的行号不对,请重新输选择\n");
		return head;
	}
	i = 1;
	pre = now = head;
	while (now->next != NULL && i != k) {
		i++;
		pre = now;
		now = pre->next;
	}//i<=k
	if(i==k){//k<=sum
		printf("%d:%s\n", k, now->name);
		printf("请输入新内容：");
		scanf(" ");
		gets(now->name);
		my_strcat(now->name,"\n");

	}else{//k>sum
		printf("已超过最大行号，默认修改最后一行\n");
		printf("%d:%s\n", i, now->name);
		printf("请输入新内容：");
		scanf(" ");
		gets(now->name);
		my_strcat(now->name,"\n");
	}
	return head;
}
/*查找字符串*/
void my_search(struct stud_node *head)
{
	char q[100],m[100],flag[20]="Y";//q为待查找字符串,即m为母串，q为子串
	int count = 0, i, j, k, n1, n2, sign=0,sum=0;
	struct stud_node *pre, *now;

	for(pre=head;pre!=NULL;pre=pre->next){//遍历链表，sum计算链表有多少个结点
		sum++;
	}
	if(sum==0||head==NULL){
		printf("此文本为空，无可查找字符\n");
	}
	else{
		printf("请输入待查找的字符串：\n");
		scanf("\n");
		gets(q);
		pre = now = head;
		k = 1;//行
		while (now != NULL && strcmp(flag,"N")!=0 && k<sum) {//一行一行比较
			n1 = my_strlen(now->name);//行的长度
			n2 = my_strlen(q);//要查找的字符串长度
			my_strcpy(m, now->name);
			/*在m中找q*/
			for (i = 0; i < n1; i++) {//在一行内查找
				j = 0;//刷新j
				if (m[i] == q[0]) {//m的第i个字符与q第0个字符比较
					for (j = 0; j < n2; j++) {//第一个相等，判断后续是否也相等
						if (m[i + j] != q[j]) {//m的第i+j个字符与q的第j个字符比较
							break;
						}
					}
				}
				if (j == n2) {//正常退出循环，说明全部相等，找到了
					printf("第%d行：%s\n", k, m);
					count++; sign = 1;
					printf("第%d个字符处找到。继续查找吗<Y/N>?", count);
					gets(flag);
				}
				if (strcmp(flag,"N")==0) {//flag等于N
					break;
				}
				else {
					sign = 0;
				}
			}
			pre = now;
			now = pre->next;
			k++;
		}
		if (sign == 0) {
			printf("没找到\n");
		}
	}
}
/*替换字符串*/
struct stud_node *my_replace(struct stud_node *head)
{
	char q[100], t[100], m[100], flag[20], flag2[20];//q为待替换字符串,即m为母串，q为子串，q将替换为t, flag用于决定是否替换， flag2用于决定是否继续替换
	int count = 0, i, j, k, n1, n2, n3, sign = 0,sum=0;//sign用来表示是否找到
	struct stud_node *pre, *now;

	for(pre=head;pre!=NULL;pre=pre->next){//遍历链表，sum计算链表有多少个结点
		sum++;
	}
	if(sum==0||head==NULL){
		printf("此文本为空，无可替换字符\n");
		return head;
	}
	printf("请输入待替换的字符串：");
	scanf("%s", q);
	printf("替换为(暂时只能接受相同字符数量):");
	scanf("%s", t);
	pre = now = head;
	k = 1;//行
	while (now != NULL && k<sum) {//一行一行比较
		n1 = my_strlen(now->name);
		n2 = my_strlen(q);
		n3 = my_strlen(t);
		my_strcpy(m, now->name);//m用来暂时储存该行的数据
		/*在m中找q*/
		for (i = 0; i < n1; i++) {//在一行内查找
			j = 0;//刷新j
			my_strcpy(flag,"N");
			my_strcpy(flag2,"N");
			if (m[i] == q[0]) {//m的第i个字符与q第0个字符比较
				for (j = 0; j < n2; j++) {//第一个相等，判断后续是否也相等
					if (m[i + j] != q[j]) {//m的第i+j个字符与q的第j个字符比较
						break;
					}
				}
			}
			if (j == n2) {//正常退出循环，说明全部相等，找到了
				printf("第%d行：%s\n", k, m);
				count++; sign = 1;
				printf("第%d个字符处找到。是否替换<Y/N>?", count);
				scanf(" ");
				gets(flag);
			}
			if (my_strcmp(flag,"Y")==0) {//要替换
				for (j = 0; j < n3; j++) {//把m中的q替换成t
					m[i + j] = t[j];
				}
				my_strcpy(now->name,m);//暂时只能替换相同数量的
				printf("替换成功,继续替换吗<Y/N>?");
				scanf(" ");
				gets(flag2);
			}
			if (my_strcmp(flag2,"N")==0 && my_strcmp(flag,"Y")==0) {//退出for循环
				break;
			}
			else {//继续替换接着找 或者 没找到接着找 或者 不替换接着找
				sign = 0;
			}
		}
		if (my_strcmp(flag2,"N")==0 && my_strcmp(flag,"Y")==0) {//退出while循环
				break;
			}
		pre = now;
		now = pre->next;
		k++;
	}
	if (sign == 0) {
		printf("没找到\n");
	}
	return head;
}
/*存盘退出*/
void my_save(struct stud_node *head, FILE *fp, char *fname)
{
	struct stud_node *now;

	now = head;
	fclose(fp);
	fp = fopen(fname, "w");//改变文件属性，切换为写操作
	//将链表内容输入到文件中
	while (now != NULL) {
		fprintf(fp, "%s", now->name);
		now = now->next;
	}
	if (fclose(fp) == 0) {
		printf("Press any key to continue");
		exit(0);
	}
}
/*放弃编辑*/
void my_give_up(FILE *fp, struct stud_node *head)
{
    fclose(fp);
    printf("Press any key to continue");
	exit(0);
}
/*我的字符串比较*/
int my_strcmp(const char *str1, const char *str2)
{
	int num;
	while(!(num=*(unsigned char*)str1-*(unsigned char*)str2) && *str1){
		str1++;//用unsigned char*  强转因为相减的字符ASC码值为正数没有负数，也就是无符号数
		str2++;//字符相等继续比较，字符不相等就跳出循环
	}//当两个字符串相同时，比较到最后一个字符\0  字符相减num==0，取反成立，所以就要看str1或者str2的字符，如果是\0表示0为假跳出循环

	if(num<0){
		return -1;
	}
	else if(num>0){
		return 1;
	}
	else{
		return 0;
	}
}
 /*我的字符串长度测量*/
int my_strlen(char *p)
{
	int count=0;
	while(*p){
		p++;
		count++;
	}
	return count;
}
/*我的字符串复制*/
char *my_strcpy(char*str1,const char*str2)
{
	char*str3 = str1;//str3用来保存str1起始地址
	while(*str1 = *str2){//一个一个复制
		str1++;
		str2++;
	}
	return str3;//返回数组起始地址
}