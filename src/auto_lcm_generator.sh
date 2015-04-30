 #!/bin/bash
 
rm -r ./lcm_headers

cpp_path='./lcm_headers/cpp'
java_path='./lcm_headers/java'
python_path='./lcm_headers/python'

for file in ./../lcmtypes/*.lcm
do
	filename=${file%.*}
	filename=${filename##*/}
	
	lcm-gen -x $file --cpp-hpath ${cpp_path}
	lcm-gen -j $file --jpath ${java_path}
	lcm-gen -p $file --ppath ${python_path}
	
	
	echo 'generated '$filename
	
	
done

for file in ${cpp_path}/avionics/*
do
	filename=${file%.*}
	filename=${filename##*/}
	echo "#include "\"avionics/${filename}.hpp"\"">>${cpp_path}/all_cpp_lcm_types.hpp
done
