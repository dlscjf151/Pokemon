#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

typedef struct inventory{
	char item[20];
	int count;
	int money;
} inv;
typedef struct pokemon{
	char name[20];
	char type[20];
	int power;
	int hp;
	int maxhp;
} pk;

int pk_load(pk **pk_s){
	int num_pk;
	FILE *fp = fopen("Pokemon.txt", "rt");
	fscanf(fp, "%d", &num_pk);
	*pk_s = (pk*)calloc(num_pk,sizeof(pk));
	for(int i =0;i<num_pk;i++){
		fscanf(fp,"%*d %*c %s %*c %s %*c %d %*c %d ",(*pk_s)[i].name, (*pk_s)[i].type, &(*pk_s)[i].power,&(*pk_s)[i].hp);
		(*pk_s)[i].maxhp = (*pk_s)[i].hp;
	}
	fclose(fp);
	return num_pk;
}
void wait(int seconds) {
	clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while (clock() < endwait);
}													//타이머 완성
void selection(pk* user_pk, pk* pk_s){
	int key =0;
	srand(time(NULL));
	printf("\n============================\n");
	printf("어느 포켓몬을 고르시겠습니까?\n");
	printf("1. 파이리 2. 이상해씨 3. 꼬부기\n>>");
	while(key>'3'||key<'0'){
		while(getchar()!='\n');
		key = getchar();
	}
	strcpy(user_pk[0].name, pk_s[key-49].name);
	strcpy(user_pk[0].type, pk_s[key-49].type);
	user_pk[0].power = 500;
	user_pk[0].maxhp = user_pk[0].hp = 2000;
}																							//포켓몬 처음 선택 완성
void load(pk *user_pk, inv *item){
	int num_save;
	FILE *load = fopen("savefile.txt", "rt");
	fscanf(load,"%d", &num_save);
	for(int i = 0;i<num_save;i++)
		fscanf(load,"%*d %*c %s %*c %s %*c %d %*c %d %*c %d",user_pk[i].name, user_pk[i].type, &user_pk[i].power,&user_pk[i].hp, &user_pk[i].maxhp);
	for(int j = 0;j<2;j++)
		fscanf(load, "%s %*c %d", item[j].item, &item[j].count);
	fscanf(load, "%d", &item[0].money);
	
	fclose(load);
}																							//세이브파일 로드 완성
pk mk_rand_pk(pk *pk_s, int pk_num){
	pk rand_pk;
	strcpy(rand_pk.name,pk_s[pk_num].name);
	strcpy(rand_pk.type,pk_s[pk_num].type);
	rand_pk.maxhp = rand_pk.hp = rand()%(701)+pk_s[pk_num].hp;	
	rand_pk.power = rand()%(101)+pk_s[pk_num].power;
	return rand_pk;
}																							//랜덤 포켓몬 생성 완성
int attack(pk *rand_pk,pk *user_pk){
	char user_type[20]; strcpy(user_type, user_pk->type);
	char com_type[20];  strcpy(com_type, rand_pk->type);
	wait(1);
	if(strcmp(user_type,com_type)==0){
		rand_pk->hp -= user_pk->power;
		if(!rand_pk->hp <=0)
			user_pk->hp -= rand_pk->power;
		printf("\n");
	}
	else if(strcmp(user_type,"불")==0 && strcmp(com_type,"풀")==0
		||strcmp(user_type,"물")==0 && strcmp(com_type,"불")==0
		||strcmp(user_type,"풀")==0 && strcmp(com_type,"물")==0){
		rand_pk->hp -= user_pk->power*1.5;
		if(!rand_pk->hp <=0)
			user_pk->hp -=rand_pk->power*0.5;
		printf("\n효과는 굉장했다!\n");
	}
	else {
		rand_pk->hp -= user_pk->power*0.5;
		if(!rand_pk->hp <=0)	
			user_pk->hp -= rand_pk->power*1.5;
		printf("\n효과가 별로인듯 하다!\n");
	}
	wait(0.7);
	if(rand_pk->hp == 0){
		printf("앗! 포켓몬이 도망가버렸다!\n");
		return 0;
	}
	else if(rand_pk->hp<0){
		printf("%s  %d/%d  vs  %s  %d/%d\n\n", user_pk->name, user_pk->hp,user_pk->maxhp,
			rand_pk->name, 0,rand_pk->maxhp);
		printf("포켓몬을 쓰러뜨렸다!\n");
		return 2;
	}
	else if(user_pk->hp<=0){
		printf("%s  %d/%d  vs  %s  %d/%d\n\n", user_pk->name, 0,user_pk->maxhp,
			rand_pk->name, rand_pk->hp,rand_pk->maxhp);
		return 1;
	}
	printf("%s  %d/%d  vs  %s  %d/%d\n\n", user_pk->name, user_pk->hp,user_pk->maxhp,
			rand_pk->name, rand_pk->hp,rand_pk->maxhp);
	return 10;
}
void add_pk(pk *user_pk,pk rand_pk,int num_user_total){
		strcpy(user_pk[num_user_total].name,rand_pk.name);
		strcpy(user_pk[num_user_total].type,rand_pk.type);
		user_pk[num_user_total].maxhp = user_pk[num_user_total].hp = rand_pk.maxhp;
		user_pk[num_user_total].power = rand_pk.power;
}
int run(pk rand_pk){
	int hp_percent = ((double)rand_pk.hp/rand_pk.maxhp)*100;
	int num_rand = rand()%10;
	if(hp_percent == 100){
		if(num_rand==0){
			wait(1);
			return 1;
		}
	}
	else if(hp_percent >=50){
		if((num_rand)<=3){
			wait(1);
			return 1;
		}
	}
	else if(hp_percent >=25){
		if((num_rand)<=6){
			wait(1);
			return 1;
		}
	}
	else{ 
		if((num_rand)<=8){
			wait(1);
			return 1;
		}
	}
	return 0;
}																							//공격함수 완성
int throw_ball(pk rand_pk,pk * pk_name,inv *item){
	double hp_percent = (double)rand_pk.hp/rand_pk.maxhp*100;
	printf("가랏! 몬스터볼!\n");
	item[0].count --;
	wait(2);
	if(hp_percent>50){
		printf("체력을 더 깍아주세요\n");
	}
	else if(hp_percent>=30){
		if(rand()%10<=2){
			printf("포켓몬을 잡았다! 이름을 입력해 주세요\n");
			scanf("%s", pk_name->name);
			return 1;
		}
	}
	else if(hp_percent >=10){
		if(rand()%10<=5){
			printf("포켓몬을 잡았다! 이름을 입력해 주세요\n");
			scanf("%s", pk_name->name);
			return 1;
		}
	}
	else{
		if(rand()%10<=8){
			printf("포켓몬을 잡았다! 이름을 입력해 주세요\n");
			scanf("%s", pk_name->name);
			return 1;
		}
	}
	printf("포켓몬이 몬스터볼을 빠져나왔다!\n");
	return 10;
}
void fight(pk*pk_s,pk * user_pk, int num,int num_user_total, inv *item){
	pk * rand_user_pk;
	int pk_num;
	int key, prev_key=0;
	int maxhp;
	int num_user_rand;
	pk rand_pk;
	int result, a=1,count=1;
	do 
		num_user_rand = rand()%num_user_total;
	while(user_pk[num_user_rand].hp<=0);
	printf("\n============================\n");
	printf("       길을 걷는중...       \n");
	srand(time(NULL));
	wait((double)(rand()%6+1)/2);
	pk_num = rand()%num;
	rand_user_pk = &user_pk[num_user_rand];
	rand_pk = mk_rand_pk(pk_s, pk_num);
	printf("\n============================\n");
	printf(" 야생의 포켓몬이 나타났다!  \n");
	while(1){
		printf("1.공격 2.도망치기 3.가방열기\n");
		do{
			while(getchar()!='\n');
			key = getchar();
			if(key =='2'&& prev_key=='2')
				printf("도망을 연속으로 시도할 수 없습니다.\n");}
		while(key!='1' && key!='2' &&key!='3' || (key=='2'&& prev_key=='2'));
		prev_key =key;
		if(key == '1'){
			result = attack(&rand_pk, rand_user_pk);
			if(result == 0){
				wait(2);
				return;
			}
			else if(result == 1){	
				a =1;
				printf("\n============================\n");
				for(int i = 0;i<num_user_total;i++)
					if(user_pk[i].hp>0){
						printf("%d. %s	%s	%d/%d\n",a, user_pk[i].name,user_pk[i].type, user_pk[i].hp,user_pk[i].maxhp);
						a++;
					}
				if(a == 1){
					printf("눈앞이 깜깜해졌다.\n");
					wait(2);
					printf("보유하고계신 모든 포켓몬의 체력을 회복하였습니다.\n");
					wait(2);
					for(int i = 0;i<num_user_total;i++)
						user_pk[i].hp = user_pk[i].maxhp;	
					return;
				}
				key =-1;
				while(!(key-48>0&&key-48<a)){
					while(getchar()!='\n');
					key =getchar();
				}
				count =1;
				for(int j = 0;j<num_user_total;j++)
					if(user_pk[j].hp>0){
						count++;
						if(count == key-48){
							rand_user_pk = &user_pk[j];
							break;
						}
					}
			}
			
			else if(result == 2){
				add_pk(user_pk,rand_pk,num_user_total);
				wait(2);
				return;

			} 
		}
		else if(key =='2'){
			int run_result = run(rand_pk);
			if(run_result == 1)
				return;
		}
		else if(key == '3'){
			printf("\n============================\n");
			printf("1. %s	x%d\n", item[0].item, item[0].count);
			printf("2. %s	x%d\n", item[1].item, item[1].count);
			do{
				while(getchar()!='\n');
				key = getchar();
				if(item[0].count == 0&& key == '1' || item[1].count == 0&& key == '2')
					printf("개수가 부족합니다.\n");}
			while(key!='0' && key!='1' && key!= '2' || (item[0].count == 0&& key == '1' || item[1].count == 0&& key == '2'));
			if(key == '0'){
				continue;
			}
			else if(key == '1'){
				result = throw_ball(rand_pk,&rand_pk,item);
				if(result == 1){
					add_pk(user_pk, rand_pk, num_user_total);
					return;
				}
			}
			else if(key == '2'){
				rand_user_pk->hp = (int)(rand_user_pk->hp*1.3);
				item[1].count --;
				if(rand_user_pk->hp >rand_user_pk->maxhp)
					rand_user_pk->hp = rand_user_pk->maxhp;
				continue;
			}
		}
	}
}	
void shopping(inv * item){
	int key;
	while(1){
		printf("\n============================\n");
		printf("1. %s	%d\n", item[0].item, 1000);
		printf("2. %s	%d\n", item[1].item, 2500);
		printf(" 보유 금액 : %d\n",item[0].money); 
		do{
			while(getchar()!='\n');
			key = getchar();}
		while(key!='0'&& key!='1'&&key!='2');
		if(key == '1'){
			if(item[0].money >= 1000){
				item[0].count++;
				item[0].money -= 1000;
			}
			else printf("돈이 부족합니다\n");
		}
		else if(key == '2'){
			if(item[0].money >=2500){
				item[1].count++;
				item[0].money -= 2500;
			}
			else printf("돈이 부족합니다\n");
		}
		else return;
	}	
}

int user_total_pk(pk*user_pk){
	int i;
	for(i = 0;i<6;i++)
		if(user_pk[i].maxhp ==0)
			break;
	return i;
}
void init (inv **item){
	(*item) = (inv*)calloc(2, sizeof(inv));
	strcpy((*item)[0].item ,"몬스터볼");
	strcpy((*item)[1].item ,"회복물약");
	(*item)[0].count =0;
	(*item)[1].count =0;
	(*item)[0].money = 10000;
	return;
}
int main(){
	int key=0;
	FILE *save;
	inv *item;
	int num,num_user_total;
	pk * pk_s;
	pk * user_pk;
	num = pk_load(&pk_s);
	user_pk = calloc(6,sizeof(pk));
	printf("============================\n");
	printf("         포켓몬스터         \n");
	printf("  press enter key to start  \n");
	printf("============================\n");
	while(getchar()!=10);
	if(fopen("savefile.txt", "rt") != NULL){
		printf("\n============================\n");
		printf("  1. 새로하기 2. 이어하기   \n");
		while(key !='1' && key!='2'){
			key = getchar();
		}
		if(key == '1'){
			selection(user_pk,pk_s);
			init(&item);
		}
		else if(key == '2'){
			item = calloc(2,sizeof(inv));
			load(user_pk, item);
		}
	}
	else{
		selection(user_pk, pk_s);
		init(&item);
	}
	while(1){
		printf("\n============================\n");
		printf("    여행을 떠나시겠습니까?  \n");
		printf("1. 네  2. 아니요  3. 상점가기 \n");
		do{
			while(getchar()!='\n');
			key=getchar();}
		while(key !='1' && key!='2'&&key!='3');
		if(key == '1'){
			num_user_total = user_total_pk(user_pk);
			fight(pk_s,user_pk,num,num_user_total, item);
		}
		else if(key == '2'){
			printf("저장하시겠습니까?\n");
			printf("1. 예   2.아니요\n");
			do{
				while(getchar()!='\n');
				key = getchar(); }
			while(key!='0'&&key !='1'&& key!='2');
			if(key == '1'){
				save = fopen("savefile.txt", "wt");
				num_user_total = user_total_pk(user_pk);
				fprintf(save, "%d\n", num_user_total);
				for(int i =0;i<num_user_total;i++)
					fprintf(save, "%d_%s | %s | %d | %d/%d\n", i+1, user_pk[i].name, user_pk[i].type, user_pk[i].power, user_pk[i].hp, user_pk[i].maxhp);
				for(int j = 0;j<2;j++)
					fprintf(save, "%s | %d\n", item[j].item, item[j].count);
				fprintf(save, "%d\n", item[0].money);
				fclose(save);
				free(pk_s);
				free(user_pk);
				free(item);
				return 0;
			}
			else if (key == '2'){
				free(pk_s);
				free(user_pk);
				free(item);
				return 0;
			}
		}
		else if(key == '3'){
			shopping(item);
		}
	}
}
