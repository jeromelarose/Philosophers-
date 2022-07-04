/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utile2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jelarose <jelarose@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 14:58:25 by jelarose          #+#    #+#             */
/*   Updated: 2020/11/01 15:44:42 by jelarose         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_two.h"

void		print_msg(t_data *data, int pid, char *msg)
{
	if (data->state)
	{
		sem_wait(data->print_sema);
		if (data->state)
		{
			ft_putnbr(get_ms(data));
			write(1, " ", 1);
			ft_putnbr((pid + 1));
			ft_putstr(msg);
		}
		sem_post(data->print_sema);
	}
}

int			is_all_eat(t_data *data, int nb)
{
	int		i;
	int		v;

	i = -1;
	v = 1;
	while (++i < nb)
	{
		if (data[i].number_of_time_each_philosophers_must_eat > 0)
			v = 0;
		else
			data[i].state = 0;
	}
	if (v)
	{
		func_detach(data, nb);
		return (1);
	}
	return (0);
}

int			verif_av(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (0);
	if (!ft_is_digit_pos(av[1]) || !ft_is_digit_pos(av[2]) ||
	!ft_is_digit_pos(av[3]) || !ft_is_digit_pos(av[4]))
		return (0);
	if (ac == 6)
	{
		if (!ft_is_digit_pos(av[5]))
			return (0);
	}
	return (1);
}

void		gest_philo(t_data *data)
{
	while (data->state)
	{
		sem_wait(data->forks_sema);
		sem_wait(data->forks_sema);
		print_msg(data, data->num, " has taken a fork\n");
		print_msg(data, data->num, " has taken a fork\n");
		data->time = get_ms(data);
		print_msg(data, data->num, " is eating\n");
		while (data->state && ((get_ms(data) - data->time) <
		data->time_to_eat))
			usleep(2);
		print_msg(data, data->num, " is sleeping\n");
		if (data->state &&
		(data->number_of_time_each_philosophers_must_eat > 0))
			data->number_of_time_each_philosophers_must_eat--;
		sem_post(data->forks_sema);
		sem_post(data->forks_sema);
		while (data->state && ((get_ms(data) - data->time - data->time_to_eat) <
		data->time_to_sleep))
			usleep(2);
		print_msg(data, data->num, " is thinking\n");
	}
}

void		*func(void *d)
{
	t_data		*data;

	data = (t_data *)d;
	gest_philo(data);
	return ((void *)0);
}
