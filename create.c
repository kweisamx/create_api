#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* images(char *token)
{
	char command[300];
	//printf("%s\n",token);
	snprintf(command,sizeof(command),"curl -s -H ""X-Auth-Token:%s"" http://controller:9292/v2/images | python -m json.tool > image.txt",token);	
//	printf("\n\ncomand : %s",command);
	system(command);
	char f1[250];
	char image[250];
	char *a = image;
	FILE *f = fopen("image.txt","r"); 
	while(fgets(f1,250,f))
	{
		if(strstr(f1,"\"id\"")){
			sscanf(f1,"%*[^:]: \"%[^\"]",image);
			//printf("%s\n",f1);
			//printf("%s\n",image);
			break;
			}
	}
	fclose(f);
	system("rm image.txt");
	return a;

}

char* flavors(char *token)
{
	char command[300];
	snprintf(command,sizeof(command),"curl -s -H ""X-Auth-Token:%s"" http://controller:8774/v2.1/flavors | python -m json.tool > flavors.txt",token);
	system(command);
	char f1[250];
	char flavor[250];
	char *b = flavor;
	FILE *f2 = fopen("flavors.txt","r"); 
	while(fgets(f1,250,f2))
	{
		if(strstr(f1,"\"id\"")){
			sscanf(f1,"%*[^:]: \"%[^\"]",flavor);
			break;
			}
	}
	fclose(f2);
	system("rm flavors.txt");
	return b;
	
}
void server_create(char *token,char *image,char *flavor,char *name)
{
	FILE *getins = fopen("getins.json","w");
	//printf("%s\n %s\n %s\n",image,flavor,token);
	fprintf(getins,"{\"server\": {\"name\": \"%s\",\"imageRef\":\"%s\",\"flavorRef\": \"http://10.0.0.11:8774/v2.1/flavors/%s\"}}",name,image,flavor);
	fclose(getins);
	char command[500];
	snprintf(command,sizeof(command),"curl -s -H ""X-Auth-Token:%s"" -H ""Content-Type:application/json"" -d @getins.json http://controller:8774/v2.1/servers | python -m json.tool",token);
	system(command);
	system("rm getins.json");
}



int main(int argc,char *argv[])
{
	printf("Now you create a instance ,%s\n",argv[1]);
	system("curl -s -i -X POST -H ""Content-Type: application/json"" -d @gettoken.json http://controller:5000/v3/auth/tokens > token.txt");
	char input[250];
	char token[250];
	FILE *f = fopen("token.txt","r");
	while(fgets(input,250,f))
	{
		if(strstr(input,"X-Subject-Token:")){
			sscanf(input,"X-Subject-Token: %s",token);
			printf("your token is get... ");		
			break;
			}
	}
	fclose(f);
	system("rm token.txt");
	char image_t[250];
	char flavor_t[250];
	
	char *image=images(token);
	//printf("image :%s\n",image);
	sprintf(image_t,"%s",image);
	char *flavor=flavors(token);
	sprintf(flavor_t,"%s",flavor);
	


	puts("==================================\n");
	printf("Please wait.....\n wish you a nice day\n");
	puts("==================================\n");
	server_create(token,image_t,flavor_t,argv[1]);
	
	return 0;
	
}
