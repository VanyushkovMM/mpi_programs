file(GLOB hdrs "*.h*") #Добавляем в переменную hdrs все файлы с расширением .h
file(GLOB srcs "*.cpp")#Добавляем в переменную srcs все файлы с расширением .h

add_library(${trapez_method} STATIC ${srcs} ${hdrs}) 

if (UNIX)
    target_link_libraries(${trapez_method} pthread)
endif (UNIX)