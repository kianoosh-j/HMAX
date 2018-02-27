for i in *txt
do
	cat $i | wc -l | cat >>size
done;
