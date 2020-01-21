-- phpMyAdmin SQL Dump
-- version 4.9.1
-- https://www.phpmyadmin.net/
--
-- Počítač: 127.0.0.1
-- Vytvořeno: Stř 08. led 2020, 22:22
-- Verze serveru: 10.4.8-MariaDB
-- Verze PHP: 7.3.11

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
-- Struktura tabulky `fingerprints`
--

CREATE TABLE `fingerprints` (
  `id` int(11) NOT NULL,
  `jmeno` varchar(50) DEFAULT NULL,
  `prijmeni` varchar(50) DEFAULT NULL,
  `otisk` text DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Vypisuji data pro tabulku `fingerprints`
--

INSERT INTO `fingerprints` (`id`, `jmeno`, `prijmeni`, `otisk`) VALUES
(10, 'Patrik', 'Bartos', 'ef,1,ff,ff,ff,ff,2,0,82,3,3,5a,17,4,1,35,1,8b,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,2,0,85,0,0,0,c0,3,30,cc,f3,f3,cc,ff,cf,ef,ff,fe,ee,ba,eb,aa,aa,aa,a6,99,66,66,55,95,55,55,51,55,11,11,44,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,14,df,ef,1,ff,ff,ff,ff,2,0,82,21,9,e6,7e,46,b1,68,1e,66,b8,d0,1e,4d,bf,d2,be,50,d,21,ff,4e,95,d,5f,54,1d,a3,7f,23,a0,1a,1f,63,20,e,ff,5c,24,25,5f,20,24,58,ff,44,a7,54,1f,51,b9,13,5f,29,c0,95,3f,1f,91,42,7d,66,2e,4f,1d,5b,2e,d1,ba,63,b1,e4,7a,24,d,a6,18,28,e,a5,d8,56,a9,d0,98,57,2c,a5,78,4d,48,51,1f,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,fb,ef,1,ff,ff,ff,ff,2,0,82,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,84,ef,1,ff,ff,ff,ff,2,0,82,3,3,5a,1a,0,1,20,1,8b,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,2,0,7f,0,0,0,0,cc,33,cc,fc,cf,3f,ff,f3,ff,fb,bb,ae,ba,aa,aa,aa,69,99,99,99,59,55,55,55,15,51,10,44,40,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'),
(11, 'Levy', 'Ukazovacek', 'ef,1,ff,ff,ff,ff,2,0,82,3,3,5c,21,4c,1,4a,1,87,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,97,0,c,0,0,c,33,0,c3,c,cf,fc,ff,3f,ff,f3,ff,bf,be,ee,ea,a6,aa,aa,6a,a9,9a,aa,65,96,55,59,55,55,55,55,15,11,10,40,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,15,13,ef,1,ff,ff,ff,ff,2,0,82,65,10,8d,5e,31,9e,43,3e,1e,25,5,3e,5e,ad,25,7e,27,30,1a,fe,f,31,9b,9e,70,b2,90,5e,4b,40,29,9e,6c,c2,10,3e,14,91,85,7f,64,1b,a3,7f,5e,a4,4e,bf,d,24,dc,df,2f,ab,9b,3f,65,b5,26,bf,4d,b5,d5,7f,10,3a,9a,3f,2b,88,6,3c,2a,8b,86,1c,3c,92,a2,9d,5e,b9,d1,ba,37,8a,20,3b,33,90,9d,1b,34,93,9e,7b,38,c,61,18,5d,3d,67,38,39,99,e7,59,61,3e,52,b9,37,18,e0,f7,6e,c9,51,3e,7d,4a,8e,9e,82,1e,4d,9f,34,2e,ef,1,ff,ff,ff,ff,2,0,82,57,c8,d4,1f,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,96,ef,1,ff,ff,ff,ff,2,0,82,3,3,5a,1b,0,1,20,1,86,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,81,0,0,c0,0,0,c0,33,3f,3c,ff,3f,cf,fc,ff,ff,be,fa,ea,aa,aa,aa,aa,99,a6,56,55,55,55,55,55,55,54,44,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'),
(12, 'Levy', 'Ukazovacek', 'ef,1,ff,ff,ff,ff,2,0,82,3,3,59,24,18,1,54,80,7e,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,d,0,1,0,87,0,c,0,c,3,33,33,f3,3f,ff,ff,ff,ff,ff,fb,ff,aa,aa,aa,aa,aa,a5,59,95,55,95,55,95,55,55,51,14,54,40,40,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,13,f4,ef,1,ff,ff,ff,ff,2,0,82,2e,96,2,de,68,97,8c,5e,3a,27,83,3e,69,2b,8d,de,28,2e,45,1e,33,b9,5a,3e,2e,d0,aa,fe,66,50,94,7e,70,50,ec,3e,6a,23,e2,7f,3a,b4,9a,7f,6b,34,a4,1f,7a,b6,8f,7f,73,3a,65,3f,5a,bd,95,1f,59,c8,a9,5f,38,cf,16,3f,7b,c6,4f,bc,78,48,e6,3c,43,24,a5,da,3b,14,1d,db,39,9b,5c,9b,42,1c,a1,bb,3c,1e,20,7b,40,21,65,5b,3d,16,df,58,6c,40,51,18,6b,c3,26,36,6e,c4,52,17,40,8,93,5e,76,9b,e2,7e,57,8d,e,1f,35,72,ef,1,ff,ff,ff,ff,2,0,82,62,11,cc,9f,73,93,8b,df,48,d,d,fc,41,c,90,da,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,e7,ef,1,ff,ff,ff,ff,2,0,82,3,3,52,1d,0,1,20,1,7d,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,b,0,1,0,79,0,0,0,30,c,33,3f,33,f3,ff,ff,ff,ff,ff,ee,aa,aa,aa,aa,9a,a5,65,55,55,55,56,65,55,45,44,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,');

--
-- Klíče pro exportované tabulky
--

--
-- Klíče pro tabulku `fingerprints`
--
ALTER TABLE `fingerprints`
  ADD PRIMARY KEY (`id`),
  ADD KEY `ix_fingerprints_prijmeni` (`prijmeni`),
  ADD KEY `ix_fingerprints_jmeno` (`jmeno`);

--
-- AUTO_INCREMENT pro tabulky
--

--
-- AUTO_INCREMENT pro tabulku `fingerprints`
--
ALTER TABLE `fingerprints`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=16;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;