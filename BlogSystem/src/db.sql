create database if not exists blog_system ;
use blog_system;

create table if not exists table_tag(
    id int primary key auto_increment,
    name varchar(32) unique  comment "用户名称"
);

create table if not exists table_user(
    id int primary key auto_increment,
    name varchar(32) unique  comment"标签名称"
);

create table if not exists table_blog(
    id int primary key auto_increment comment"博客id",
    tag_id int comment"标签id",
    user_id int comment "用户id",
    title varchar(255) comment "标题",
    content text comment "文章内容",
    ctime datetime comment "最后修改时间",
    foreign key (tag_id) references table_tag(id),
    foreign key (user_id) references table_user(id)
);
