-- phpMyAdmin SQL Dump
-- version 4.9.0.1
-- https://www.phpmyadmin.net/
--
-- Počítač: 127.0.0.1
-- Vytvořeno: Úte 05. lis 2019, 18:53
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

-- --------------------------------------------------------

--
-- Struktura tabulky `fingerprints`
--

CREATE TABLE `fingerprints` (
  `id` int(10) UNSIGNED NOT NULL,
  `jmeno` varchar(45) DEFAULT NULL,
  `prijmeni` varchar(45) DEFAULT NULL,
  `otisk` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Vypisuji data pro tabulku `fingerprints`
--

INSERT INTO `fingerprints` (`id`, `jmeno`, `prijmeni`, `otisk`) VALUES
(26, 'test', 'test', '239125525525525520343384250132113700000000000000000000000166239125525525525520340000000000000000901012300034851512522552512515512391255255255255203425423823823423817017017125023310185858585698517810640000000000011173239125525525525520340000000000000000000000000000000003623912552552552552034601219712638177012618279294107301021908116586254954742190754843158711210512711125239125525525525520341914315731552515525547282191271516315515913191942556766512371146232516614623621914177239125525525525520342318431556418010824815579566984125661782155416182134246191811951839507711412128239125525525525520341044692430000000000000000000000000000114623912552552552552034000000000000000000000000000000000362391255255255255203400000000000000000000000000000000036239125525525525520340000000000000000000000000000000003623912552552552552034000000000000000000000000000000000362391255255255255203433961101321143');

--
-- Klíče pro exportované tabulky
--

--
-- Klíče pro tabulku `attendance`
--
ALTER TABLE `attendance`
  ADD KEY `fk_idx` (`id`);

--
-- Klíče pro tabulku `fingerprints`
--
ALTER TABLE `fingerprints`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT pro tabulky
--

--
-- AUTO_INCREMENT pro tabulku `fingerprints`
--
ALTER TABLE `fingerprints`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=28;

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
