-- phpMyAdmin SQL Dump
-- version 4.9.0.1
-- https://www.phpmyadmin.net/
--
-- Počítač: 127.0.0.1
-- Vytvořeno: Sob 28. zář 2019, 17:08
-- Verze serveru: 10.4.6-MariaDB
-- Verze PHP: 7.3.9

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Databáze: `attendance`
--

-- --------------------------------------------------------

--
-- Struktura tabulky `attendance`
--

CREATE TABLE `attendance` (
  `id` int(10) UNSIGNED NOT NULL,
  `prijmeni` varchar(45) DEFAULT NULL,
  `jmeno` varchar(45) DEFAULT NULL,
  `datum` date DEFAULT NULL,
  `cas` time DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Klíče pro exportované tabulky
--

--
-- Klíče pro tabulku `attendance`
--
ALTER TABLE `attendance`
  ADD KEY `fk_idx` (`id`);

--
-- Omezení pro exportované tabulky
--

--
-- Omezení pro tabulku `attendance`
--
ALTER TABLE `attendance`
  ADD CONSTRAINT `fk_id` FOREIGN KEY (`id`) REFERENCES `fingerprints` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
