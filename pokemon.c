#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>


typedef struct inventory {
	char item[20];
	int count;
} inv;
typedef struct pokemon {
	char name[20];
	char type[20];
	int power;
	int hp;
	int maxhp;
} pk;

int pk_load(pk **pk_s) {
	int num_pk;
	FILE *fp = fopen("Pokemon.txt", "rt");
	fscanf(fp, "%d", &num_pk);
	*pk_s = (pk*)calloc(num_pk, sizeof(pk));
	for (int i = 0; i<num_pk; i++) {
		fscanf(fp, "%*d %*c %s %*c %s %*c %d %*c %d ", (*pk_s)[i].name, (*pk_s)[i].type, &(*pk_s)[i].power, &(*pk_s)[i].hp);
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
void selection(pk* user_pk, pk* pk_s) {
	int key = 0;
	srand(time(NULL));
	printf("\n============================\n");
	printf("어느 포켓몬을 고르시겠습니까?\n");
	printf("1. 파이리 2. 이상해씨 3. 꼬부기\n>>");
	while (key>'3' || key<'0') {
		while (getchar() != '\n');
		key = getchar();
	}
	strcpy(user_pk[0].name, pk_s[key - 49].name);
	strcpy(user_pk[0].type, pk_s[key - 49].type);
	user_pk[0].power = (rand() % 51) + pk_s[key - 48].power;
	user_pk[0].maxhp = user_pk[0].hp = (rand() % 501) + pk_s[key - 48].hp;
}																							//포켓몬 처음 선택 완성
void load(pk *user_pk) {
	FILE *load = fopen("savefile.txt", "rt");
	int a=0;
	int i=0;
	while (a != EOF) {
		a = fscanf(load, "%*d %*c %s %*c %s %*c %d %*c %d", user_pk[i].name, user_pk[i].type, &user_pk[i].power, &user_pk[i].hp);
		i++;
	}
}																							//세이브파일 로드 완성
pk mk_rand_pk(pk *pk_s, int pk_num) {
	pk rand_pk;
	strcpy(rand_pk.name, pk_s[pk_num].name);
	strcpy(rand_pk.type, pk_s[pk_num].type);
	rand_pk.maxhp = rand_pk.hp = rand() % (701) + pk_s[pk_num].hp;
	rand_pk.power = rand() % (101) + pk_s[pk_num].power;
	return rand_pk;
}																							//랜덤 포켓몬 생성 완성
int attack(pk *rand_pk, pk *user_pk) {
	char user_type[20]; strcpy(user_type, user_pk->type);
	char com_type[20];  strcpy(com_type, rand_pk->type);
	wait(1);
	if (strcmp(user_type, com_type) == 0) {
		rand_pk->hp -= user_pk->power;
		user_pk->hp -= rand_pk->power;
		printf("\n");
	}
	else if (strcmp(user_type, "불") == 0 && strcmp(com_type, "풀") == 0
		|| strcmp(user_type, "물") == 0 && strcmp(com_type, "불") == 0
		|| strcmp(user_type, "풀") == 0 && strcmp(com_type, "물") == 0) {
		user_pk->hp -= rand_pk->power*0.5;
		rand_pk->hp -= user_pk->power*1.5;
		printf("\n효과는 굉장했다!\n");
	}
	else {
		user_pk->hp -= rand_pk->power*1.5;
		rand_pk->hp -= user_pk->power*0.5;
		printf("\n효과가 별로인듯 하다!\n");
	}
	wait(0.7);
	if (rand_pk->hp == 0) {
		printf("앗! 포켓몬이 도망가버렸다!\n");
		return 0;
	}
	else if (user_pk->hp <= 0) {
		printf("%s  %d/%d  vs  %s  %d/%d\n\n", user_pk->name, 0, user_pk->maxhp,
			rand_pk->name, rand_pk->hp, rand_pk->maxhp);
		return 1;
	}
	else if (rand_pk->hp<0) {
		printf("%s  %d/%d  vs  %s  %d/%d\n\n", user_pk->name, user_pk->hp, user_pk->maxhp,
			rand_pk->name, 0, rand_pk->maxhp);
		printf("포켓몬을 쓰러뜨렸다!\n");
		return 2;
	}
	printf("%s  %d/%d  vs  %s  %d/%d\n\n", user_pk->name, user_pk->hp, user_pk->maxhp,
		rand_pk->name, rand_pk->hp, rand_pk->maxhp);
	return 10;
}
void add_pk(pk *user_pk, pk rand_pk, int num_user_total) {
	strcpy(user_pk[num_user_total].name, rand_pk.name);
	strcpy(user_pk[num_user_total].type, rand_pk.type);
	user_pk[num_user_total].maxhp = user_pk[num_user_total].hp = rand_pk.maxhp;
	user_pk[num_user_total].power = rand_pk.power;
}
int run(pk rand_pk) {
	int hp_percent = ((double)rand_pk.hp / rand_pk.maxhp) * 100;
	int num_rand = rand() % 10;
	if (hp_percent == 100) {
		if (num_rand == 0) {
			wait(1);
			return 1;
		}
	}
	else if (hp_percent >= 50) {
		if ((num_rand) <= 3) {
			wait(1);
			return 1;
		}
	}
	else if (hp_percent >= 25) {
		if ((num_rand) <= 6) {
			wait(1);
			return 1;
		}
	}
	else {
		if ((num_rand) <= 8) {
			wait(1);
			return 1;
		}
	}
	return 0;
}																							//공격함수 완성
void fight(pk*pk_s, pk * user_pk, int num, int num_user_total) {
	pk * rand_user_pk;
	int pk_num;
	int key, prev_key = 0;
	int num_user_rand;
	pk rand_pk;
	int result, a = 1, count = 1;
	do
		num_user_rand = rand() % num_user_total;
	while (user_pk[num_user_rand].hp <= 0);
	printf("\n============================\n");
	printf("       길을 걷는중...       \n");
	srand(time(NULL));
	wait((double)(rand() % 6 + 1) / 2);
	pk_num = rand() % num;
	rand_user_pk = &user_pk[num_user_rand];
	rand_pk = mk_rand_pk(pk_s, pk_num);
	printf("\n============================\n");
	printf(" 야생의 포켓몬이 나타났다!  \n");
	while (1) {
		printf("1.공격 2.도망치기 3.가방열기\n");
		do {
			while (getchar() != '\n');
			key = getchar();
			if (key == '2'&& prev_key == '2')
				printf("도망을 연속으로 시도할 수 없습니다.\n");
		} while (key != '1' && key != '2' &&key != '3' || (key == '2'&& prev_key == '2'));
		prev_key = key;
		if (key == '1') {
			result = attack(&rand_pk, rand_user_pk);
			if (result == 0) {
				wait(2);
				return;
			}
			else if (result == 1) {
				a = 1;
				printf("\n============================\n");
				for (int i = 0; i<num_user_total; i++)
					if (user_pk[i].hp>0) {
						printf("%d. %s	%s	%d/%d\n", a, user_pk[i].name, user_pk[i].type, user_pk[i].hp, user_pk[i].maxhp);
						a++;
					}
				if (a == 1) {
					printf("눈앞이 깜깜해졌다.\n");
					wait(2);
					printf("보유하고계신 모든 포켓몬의 체력을 회복하였습니다.\n");
					wait(2);
					for (int i = 0; i<num_user_total; i++)
						user_pk[i].hp = user_pk[i].maxhp;
					return;
				}
				key = -1;
				while (!(key - 48>0 && key - 48<a)) {
					while (getchar() != '\n');
					key = getchar();
				}
				count = 1;
				for (int j = 0; j<num_user_total; j++)
					if (user_pk[j].hp>0) {
						count++;
						if (count == key - 48) {
							rand_user_pk = &user_pk[j];
							break;
						}
					}
			}

			else if (result == 2) {
				add_pk(user_pk, rand_pk, num_user_total);
				wait(2);
				return;

			}
		}
		else if (key == '2') {
			int run_result = run(rand_pk);
			if (run_result == 1)
				return;
		}
		else if (key == '3')
		{
		}
	}
}
int user_total_pk(pk*user_pk) {
	int i;
	for (i = 0; i<6; i++)
		if (user_pk[i].maxhp == 0)
			break;
	return i;
}
void init(inv **item) {
	(*item) = (inv*)calloc(2, sizeof(inv));
	strcpy((*item)[0].item, "몬스터 볼");
	strcpy((*item)[1].item, "회복 물약");
	(*item)[0].count = 0;
	(*item)[1].count = 0;
	return;
}
int main() {
	int key = 0;
	FILE *save;
	inv *item;
	int num, num_user_total;
	pk * pk_s;
	pk * user_pk;
	num = pk_load(&pk_s);
	init(&item);
	user_pk = calloc(6, sizeof(pk));
	printf("============================\n");
	printf("         포켓몬스터         \n");
	printf("  press enter key to start  \n");
	printf("============================\n");
	while (getchar() != 10);
	if (fopen("savefile.txt", "rt") != NULL) {
		printf("\n============================\n");
		printf("  1. 새로하기 2. 이어하기   \n");
		while (key != '1' && key != '2') {
			key = getchar();
		}
		if (key == '1') {
			selection(user_pk, pk_s);
		}
		else if (key == '2') {
			load(user_pk);
		}
	}
	else {
		selection(user_pk, pk_s);
	}
	while (1) {
		printf("\n============================\n");
		printf("    여행을 떠나시겠습니까?  \n");
		printf("1. 네  2. 아니요  3. 상점가기 \n");
		do {
			while (getchar() != '\n');
			key = getchar();
		} while (key != '1' && key != '2'&&key != '3');
		if (key == '1') {
			num_user_total = user_total_pk(user_pk);
			fight(pk_s, user_pk, num, num_user_total);
		}
	}
}
