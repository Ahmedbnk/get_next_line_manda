/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenkrar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:31:33 by abenkrar          #+#    #+#             */
/*   Updated: 2024/12/09 14:24:13 by abenkrar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*p_joined;

	if (s1 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	p_joined = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (p_joined == NULL)
		return (NULL);
	ft_memcpy(p_joined, s1, s1_len);
	ft_memcpy((p_joined + s1_len), s2, s2_len);
	p_joined[s1_len + s2_len] = '\0';
	return (p_joined);
}

char	*creat_rest(int fd, char *rest)
{
	char	*buffer;
	ssize_t	readed_bytes;
	char	*rest_clean;

	readed_bytes = 1;
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	while (ft_strchr(rest, '\n') == NULL && readed_bytes != 0)
	{
		readed_bytes = read(fd, buffer, BUFFER_SIZE);
		if (readed_bytes < 1)
		{
			free(buffer);
			free(rest);
			return (NULL);
		}
		buffer[readed_bytes] = '\0';
		rest_clean = rest;
		rest = ft_strjoin(rest_clean, buffer);
		free(rest_clean);
	}
	free(buffer);
	return (rest);
}

char	*creat_line(char **rest)
{
	static char	*line;
	char		*rest_clean;
	int			i;

	i = 0;
	while (*((*rest) + i) != '\n' && *((*rest) + i) != '\0')
	{
		i++;
	}
	i++;
	line = ft_substr(*rest, 0, i);
	rest_clean = *rest;
	*rest = ft_substr(rest_clean, i, ft_strlen(rest_clean) - i);
	free(rest_clean);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*rest;
	static char	*main_line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (rest == NULL)
	{
		rest = malloc(1);
		rest[0] = '\0';
	}
	rest = creat_rest(fd, rest);
	if (rest == NULL)
		return (NULL);
	main_line = creat_line(&rest);
	return (main_line);
}

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
		return (1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (0);
}
