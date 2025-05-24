CREATE TABLE `custom_solocraft_character_stats` (
  `GUID` int unsigned NOT NULL DEFAULT '0',
  `Difficulty` float NOT NULL,
  `GroupSize` int unsigned NOT NULL DEFAULT '0',
  `SpellPower` int unsigned NOT NULL DEFAULT '0',
  `Stats` float NOT NULL DEFAULT '100',
  PRIMARY KEY (`GUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;