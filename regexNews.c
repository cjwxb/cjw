#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <regex.h>
#include <sys/mman.h>

int main(void)
{
	    char text[4096];
		char href_1[4096];
		char div_1[4096];
		const char* img = "<img src=\"//\\([^>]\\+\\?\\)\">";
		const char* href = "<a href=\"//\\([^>]\\+\\?\\)\">";
		const char* div = "<div class=\"title\">\\([^<]\\+\\?\\)</div>";//这个没加上之前，前两个可以正常检索到数据，加上了就不行了，搜到的就不全，单独拿出来还正常
		
	    regex_t Reg;
		regcomp(&Reg,img,0);
		regmatch_t ma[2];
		

		regex_t Reg_1;
		regcomp(&Reg_1,href,0);
		regmatch_t ma_1[2];

		regex_t Reg_2;
		regcomp(&Reg_2,div,0);
		regmatch_t ma_2[2];
		
		
		
		int fd,size;
		fd = open("uuuuuu",O_RDWR);
		size = lseek(fd,0,SEEK_END);
		char* data = NULL;
		
		data = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);	
		close(fd);

		int imgTemp = 0;
		int hrefTemp = 0;
		int divTemp = 0;

		imgTemp = regexec(&Reg,data,2,ma,0);
		hrefTemp = regexec(&Reg_1,data,2,ma_1,0);
		divTemp = regexec(&Reg_2,data,2,ma_2,0);
		while(imgTemp  == 0  ||  hrefTemp  == 0  ||  divTemp == 0)
		{
			bzero(text,4096);
			bzero(href_1,4096);
			bzero(div_1,4096);

			if(hrefTemp == 0){ 
				snprintf(href_1,ma_1[1].rm_eo - ma_1[1].rm_so+1,"%s",data+ma_1[1].rm_so);
			}

			if(imgTemp == 0){
				snprintf(text,ma[1].rm_eo - ma[1].rm_so+1,"%s",data+ma[1].rm_so);
			}
			
		
			if(divTemp == 0){
				snprintf(div_1,ma_2[1].rm_eo - ma_2[1].rm_so+1,"%s",data+ma_2[1].rm_so);
			}

				printf("%s \n %s \n %s \n",text,href_1,div_1);			
			if(hrefTemp == 0){
				    data+=ma_1[0].rm_eo;
			}

			if(imgTemp == 0){
				data+=ma[0].rm_eo;
			}
			if(divTemp == 0){
				data+=ma_2[0].rm_eo;
			}
			
			imgTemp = regexec(&Reg,data,2,ma,0);
		    hrefTemp = regexec(&Reg_1,data,2,ma_1,0);
			divTemp = regexec(&Reg_2,data,2,ma_2,0);

		
		}
	regfree(&Reg);
	regfree(&Reg_1);
	regfree(&Reg_2);
	return 0;
}
                                                                            

