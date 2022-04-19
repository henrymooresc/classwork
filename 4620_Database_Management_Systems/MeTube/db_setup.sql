CREATE TABLE users (
	id INT NOT NULL AUTO_INCREMENT,
	type INT, 
	username varchar(255), 
	password varchar(255), 
	email varchar(255), 
	PRIMARY KEY (id)
);

CREATE TABLE media (
	id INT NOT NULL AUTO_INCREMENT, 
	file_name varchar(255),
	type varchar(255),
	path varchar(255),
	upload_date timestamp,
	title varchar(255),
	description varchar(255),
	category varchar(255),
	view_count INT,
	uploader_id INT,
	PRIMARY KEY (id),
	FOREIGN KEY (uploader_id) REFERENCES users(id)
);

CREATE TABLE tags (
	id INT NOT NULL AUTO_INCREMENT, 
	media_id INT,
	tag varchar(255),
	PRIMARY KEY (id),
	FOREIGN KEY (media_id) REFERENCES media(id) 
);

CREATE TABLE comments (
	id INT NOT NULL AUTO_INCREMENT,  
	media_id INT, 
	user_id INT, 
	comment_text varchar(255), 
	PRIMARY KEY (id), 
	FOREIGN KEY (media_id) REFERENCES media(id),
	FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE contacts (
	id INT NOT NULL AUTO_INCREMENT,
	label varchar(255),
	user_id INT,
	contact_id INT, 
	PRIMARY KEY (id), 
	FOREIGN KEY (user_id) REFERENCES users(id), 
	FOREIGN KEY (contact_id) REFERENCES users(id)
);

CREATE TABLE messages (
	id INT NOT NULL AUTO_INCREMENT, 
	to_id INT,
	from_id INT,
	content varchar(255),
	PRIMARY KEY (id),
	FOREIGN KEY (to_id) REFERENCES users(id),
	FOREIGN KEY (from_id) REFERENCES users(id)
);

CREATE TABLE favorites (
	id INT NOT NULL AUTO_INCREMENT, 
	user_id INT,
	media_id INT,
	PRIMARY KEY (id),
	FOREIGN KEY (user_id) REFERENCES users(id), 
	FOREIGN KEY (media_id) REFERENCES media(id)
);

CREATE TABLE playlists (
	id INT NOT NULL AUTO_INCREMENT, 
	name varchar(255),
	first BOOLEAN,
	user_id INT,
	media_id INT,
	next_id INT,
	PRIMARY KEY (id),
	FOREIGN KEY (user_id) REFERENCES users(id), 
	FOREIGN KEY (media_id) REFERENCES media(id),
	FOREIGN KEY (next_id) REFERENCES playlists(id)
);

CREATE TABLE subscriptions (
	id INT NOT NULL AUTO_INCREMENT,
	sub_id INT,
	sub_to_id INT,
	PRIMARY KEY (id),
	FOREIGN KEY (sub_id) REFERENCES users(id),
	FOREIGN KEY (sub_to_id) REFERENCES users(id)
);