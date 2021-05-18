-- phpMyAdmin SQL Dump
-- version 5.0.1
-- https://www.phpmyadmin.net/
--
-- Host: mysql-server
-- Tempo de geração: 18-Maio-2021 às 11:15
-- Versão do servidor: 8.0.19
-- versão do PHP: 7.4.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Banco de dados: `sdi_project`
--

-- --------------------------------------------------------

--
-- Estrutura da tabela `chats`
--

CREATE TABLE `chats` (
  `chatID` int NOT NULL,
  `text` text CHARACTER SET utf8 COLLATE utf8_general_ci
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `chats`
--

INSERT INTO `chats` (`chatID`, `text`) VALUES
(1, NULL),
(2, NULL),
(3, '\nkiko: hello'),
(4, '\nkiko: Hello\nuser: How r you?'),
(5, NULL),
(6, NULL),
(7, NULL),
(8, NULL);

-- --------------------------------------------------------

--
-- Estrutura da tabela `groups`
--

CREATE TABLE `groups` (
  `groupID` int NOT NULL,
  `groupName` varchar(200) NOT NULL,
  `chatID` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `groups`
--

INSERT INTO `groups` (`groupID`, `groupName`, `chatID`) VALUES
(6, 'Group 1', 3),
(7, 'Group 2', 5),
(8, '', 7),
(9, 'asd', 8);

-- --------------------------------------------------------

--
-- Estrutura da tabela `users`
--

CREATE TABLE `users` (
  `userID` int NOT NULL,
  `userName` varchar(200) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `password` varchar(200) NOT NULL,
  `isOnline` tinyint(1) DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `users`
--

INSERT INTO `users` (`userID`, `userName`, `password`, `isOnline`) VALUES
(1, 'user', '6713693101708072033', 0),
(2, '123', '10089081994332581363', 0),
(3, 'jon', '10089081994332581363', 0),
(4, 'kiko', '10089081994332581363', 0),
(5, 'test', '15118982290295364091', 0),
(6, 'bg', '10089081994332581363', 0),
(7, 'abc', '10089081994332581363', 0),
(8, '', '6142509188972423790', 0);

-- --------------------------------------------------------

--
-- Estrutura da tabela `user_friends`
--

CREATE TABLE `user_friends` (
  `userID` int NOT NULL,
  `friendID` int NOT NULL,
  `accepted` tinyint(1) NOT NULL DEFAULT '0',
  `chatID` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `user_friends`
--

INSERT INTO `user_friends` (`userID`, `friendID`, `accepted`, `chatID`) VALUES
(4, 1, 1, 4),
(4, 5, 1, 6);

-- --------------------------------------------------------

--
-- Estrutura da tabela `user_groups`
--

CREATE TABLE `user_groups` (
  `userID` int NOT NULL,
  `groupID` int NOT NULL,
  `role` int NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `user_groups`
--

INSERT INTO `user_groups` (`userID`, `groupID`, `role`) VALUES
(1, 6, 0),
(1, 7, 2),
(4, 6, 2),
(4, 9, 2),
(5, 8, 2);

--
-- Índices para tabelas despejadas
--

--
-- Índices para tabela `chats`
--
ALTER TABLE `chats`
  ADD PRIMARY KEY (`chatID`);

--
-- Índices para tabela `groups`
--
ALTER TABLE `groups`
  ADD PRIMARY KEY (`groupID`),
  ADD KEY `groups_ibfk_1` (`chatID`);

--
-- Índices para tabela `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`userID`);

--
-- Índices para tabela `user_friends`
--
ALTER TABLE `user_friends`
  ADD PRIMARY KEY (`userID`,`friendID`),
  ADD KEY `friendID` (`friendID`),
  ADD KEY `user_friends_ibfk_3` (`chatID`);

--
-- Índices para tabela `user_groups`
--
ALTER TABLE `user_groups`
  ADD PRIMARY KEY (`userID`,`groupID`),
  ADD KEY `groupID` (`groupID`);

--
-- AUTO_INCREMENT de tabelas despejadas
--

--
-- AUTO_INCREMENT de tabela `chats`
--
ALTER TABLE `chats`
  MODIFY `chatID` int NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- AUTO_INCREMENT de tabela `groups`
--
ALTER TABLE `groups`
  MODIFY `groupID` int NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;

--
-- AUTO_INCREMENT de tabela `users`
--
ALTER TABLE `users`
  MODIFY `userID` int NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--
-- Restrições para despejos de tabelas
--

--
-- Limitadores para a tabela `groups`
--
ALTER TABLE `groups`
  ADD CONSTRAINT `groups_ibfk_1` FOREIGN KEY (`chatID`) REFERENCES `chats` (`chatID`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Limitadores para a tabela `user_friends`
--
ALTER TABLE `user_friends`
  ADD CONSTRAINT `user_friends_ibfk_1` FOREIGN KEY (`userID`) REFERENCES `users` (`userID`),
  ADD CONSTRAINT `user_friends_ibfk_2` FOREIGN KEY (`friendID`) REFERENCES `users` (`userID`),
  ADD CONSTRAINT `user_friends_ibfk_3` FOREIGN KEY (`chatID`) REFERENCES `chats` (`chatID`) ON DELETE RESTRICT ON UPDATE RESTRICT;

--
-- Limitadores para a tabela `user_groups`
--
ALTER TABLE `user_groups`
  ADD CONSTRAINT `user_groups_ibfk_1` FOREIGN KEY (`userID`) REFERENCES `users` (`userID`),
  ADD CONSTRAINT `user_groups_ibfk_2` FOREIGN KEY (`groupID`) REFERENCES `groups` (`groupID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
