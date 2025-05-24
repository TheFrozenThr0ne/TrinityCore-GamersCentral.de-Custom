/*

Database Actions:

1 = item
2 = gold
3 = name change
4 = faction change
5 = race change

script made by talamortis

*/

CREATE TABLE IF NOT EXISTS `reward_shop` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `action` int(11) NOT NULL DEFAULT '0',
  `action_data` int(11) NOT NULL DEFAULT '0',
  `quantity` int(11) NOT NULL DEFAULT '0',
  `code` char(50) NOT NULL,
  `status` int(11) NOT NULL DEFAULT '0',
  `PlayerGUID` int(11) NOT NULL DEFAULT '0',
  `PlayerIP` varchar(50) NOT NULL DEFAULT '0',
  `CreatedBy` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 COMMENT='Database Actions:\r\n\r\n1 = item\r\n2 = gold\r\n3 = name change\r\n4 = faction change\r\n5 = race change';

INSERT INTO reward_shop VALUES (1, 2, 11, 0, 123, 0, 0, 0,0);