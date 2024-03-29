/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_glyph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgama <mgama@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 22:03:35 by mgama             #+#    #+#             */
/*   Updated: 2024/03/24 19:33:18 by mgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_font.h"
#include "images/images.h"

void	reflect(t_glyph_ctx *ctx, int *x, int *y);

void	draw_glyph_path(t_glyph *glyph, t_glyph_ctx *ctx, t_pointi2 pos)
{
	t_glyph_point	*gpoint;
	t_glyph_point	*prev;

	gpoint = glyph->points;
	prev = NULL;
	while (gpoint)
	{
		if (ctx->pr.s == 0)
		{
			move_to(ctx, gpoint->x + pos.x, gpoint->y + pos.y);
			ctx->pr.s = 1;
		}
		else if (ctx->pr.s == 1)
		{
			if (gpoint->on_curve)
				line_to(ctx, gpoint->x + pos.x, gpoint->y + pos.y);
			else
				ctx->pr.s = 2;
		}
		else
		{
			if (gpoint->on_curve)
			{
				quadratic_curve_to(ctx,
					ft_create_ipoint(prev->x + pos.x, prev->y + pos.y),
					ft_create_ipoint(gpoint->x + pos.x, gpoint->y + pos.y));
				ctx->pr.s = 1;
			}
			else
			{
				quadratic_curve_to(ctx,
					ft_create_ipoint(prev->x + pos.x, prev->y + pos.y),
					ft_create_ipoint(
						((prev->x + gpoint->x) >> 1) + pos.x,
						((prev->y + gpoint->y) >> 1) + pos.y));
			}
		}
		if (ctx->pr.p == get_glyph_contour_by_id(glyph, ctx->pr.c)->val)
		{
			if (ctx->pr.s == 2)
			{
				prev = gpoint;
				gpoint = get_glyph_point_by_id(glyph, ctx->pr.contour_s);
				if (gpoint->on_curve)
				{
					quadratic_curve_to(ctx,
						ft_create_ipoint(prev->x + pos.x, prev->y + pos.y),
						ft_create_ipoint(gpoint->x + pos.x, gpoint->y + pos.y));
				}
				else
				{
					quadratic_curve_to(ctx,
						ft_create_ipoint(prev->x + pos.x, prev->y + pos.y),
						ft_create_ipoint(
							((prev->x + gpoint->x) >> 1) + pos.x,
							((prev->y + gpoint->y) >> 1) + pos.y));
				}
			}
			ctx->pr.contour_s = ctx->pr.p + 1;
			ctx->pr.c++;
			ctx->pr.s = 0;
			gpoint = prev;
		}
		ctx->pr.p++;
		prev = gpoint;
		gpoint = gpoint->next;
	}
}

// void	find_intersections(t_glyph *glyph, t_glyph_ctx *ctx,
// 	t_intersections *x_inter, t_pointi2 pos)
// {
// 	t_pointi2		p1;
// 	t_pointi2		p2;
// 	double			x_intersect;
// 	int				j;
// 	int				inter;
// 	int				x;

// 	inter = 0;
// 	x = -1;
// 	while (++x < ctx->width)
// 	{
// 		// Réinitialisez le compteur d'intersections pour
// 		// chaque ligne horizontale.
// 		inter = 0;

// 		j = 0;
// 		while (j < ctx->pidx)
// 		{
// 			p1 = ctx->points[j];
// 			p2 = ctx->points[(j + 1) % ctx->pidx];
// 			if (p2.x == INT32_MIN && p2.y == INT32_MIN)
// 			{
// 				j += 2;
// 				continue ;
// 			}
// 			// Vérifiez si le segment coupe la ligne horizontale
// 			if ((p1.y + pos.y <= x_inter->y && x_inter->y < p2.y + pos.y)
// 				|| (p2.y + pos.y <= x_inter->y && x_inter->y < p1.y + pos.y))
// 			{
// 				// Vérifiez si le point d'intersection est
// 				// à gauche du pixel (x, x_inter->y)
// 				if ((p1.x + pos.x < x && p2.x + pos.x >= x)
// 					|| (p2.x + pos.x < x && p1.x + pos.x >= x))
// 				{
// 					inter++;
// 				}
// 			}

// 			j += 2;
// 		}

// 		// Vérifiez si le nombre d'intersections est impair.
// 		if (inter % 2 == 1)
// 		{
// 			ft_pixel_put(ctx->pr.image, x, x_inter->y, 0xFFFFFF);
// 		}
// 	}

// 	t_glyph_point	*point = glyph->points;
// 	t_glyph_point	*next;
// 	uint32_t		id = 0;

// 	while (point)
// 	{
// 		next = point->next;
// 		if (!next)
// 			point = glyph->points;
// 		// if (point->id == get_glyph_contour_by_id(glyph, id)->val)
// 		// 	continue ;
// 		reflect(ctx, &(point->x), &(point->x));
// 		reflect(ctx, &(next->x), &(next->x));
// 		if ((point->y + pos.y <= x_inter->y && x_inter->y < next->y + pos.y)
// 			|| (next->y + pos.y <= x_inter->y && x_inter->y < point->y + pos.y))
// 		{
// 			x_intersect = point->x + (x_inter->y - point->y)
// 				* (next->x - point->x) / (next->y - point->y);
// 			printf("x_inter[%d] %f\n", x_inter->count, x_intersect);
// 			ft_pixel_put(ctx->pr.image, x_intersect, x_inter->y, 0xFFFFFF);
// 			x_inter->t[x_inter->count] = x_intersect;
// 			x_inter->count++;
// 		}
// 		point = point->next;
// 		if (point)
// 			point = point->next;
// 	}
// }

// void	fill_line(t_intersections *x_inter, t_glyph_ctx *ctx, int y, int xoff)
// {
// 	int	c;
// 	int	x;

// 	c = 0;
// 	x = xoff - 1;
// 	while (++x < ctx->glyph_width)
// 	{
// 		if (ft_get_pixel(ctx->pr.image, x, y) == 0x00FF00)
// 		{
// 			c++;
// 			// ft_pixel_put(ctx->pr.image, x, y, 0x00FF00);
// 			while (ft_get_pixel(ctx->pr.image, x, y) == 0x00FF00
// 					&& x < ctx->glyph_width)
// 				x++;
// 			printf("y=%d x=%d\n", y, x);
// 		}
// 		if (c % 2 == 1)
// 			ft_pixel_put(ctx->pr.image, x, y, ctx->color);
// 	}
// 	if (c > 0)
// 	printf("finished with c=%d\n", c);
// 	for (int i = 0; i < x_inter->count - 1; i++) {
// 		for (int j = 0; j < x_inter->count - i - 1; j++) {
// 			if (x_inter->t[j] > x_inter->t[j + 1]) {
// 				double temp = x_inter->t[j];
// 				x_inter->t[j] = x_inter->t[j + 1];
// 				x_inter->t[j + 1] = temp;
// 			}
// 		}
// 	}
// 	for (int i = 0; i < x_inter->count - 1; i++) {
// 		printf("x_inter->count=%d\n", x_inter->t[i]);
// 	}
// 	// printf("---------------------------------------------\n");
// 	for (int i = 0; i < x_inter->count - 1; i += 2) {
// 		int startX = x_inter->t[i];
// 		int endX = x_inter->t[i + 1];

// 		for (int x = startX; x <= endX; x++) {
// 			// printf("Draw pixel at x=%d\n", x);
// 			ft_pixel_put(ctx->pr.image, x, y, 0xFF | (0x7F << 24));
// 		}
// 	}
// }

// void	fill_glyph(t_glyph *glyph, t_glyph_ctx *ctx, t_pointi2 pos)
// {
// 	int				y;
// 	// t_intersections	x_intersections;

// 	y = -1;
// 	while (++y < ctx->height)
// 	// while (++y < 300)
// 	{
// 		// ft_bzero(&x_intersections, sizeof(t_intersections));
// 		// x_intersections.y = y;
// 		// find_intersections(glyph, ctx, NULL, pos);
// 		// fill_line(NULL, ctx, pos.y + y, pos.x);
// 	}
// 	y = -1;
// 	while (++y < ctx->pidx)
// 	{
// 		ft_pixel_put(ctx->pr.image, ctx->points[y].x,
// 			ctx->points[y].y, 0xFF0000);
// 	}
// }

#include <stdlib.h>

// Structure pour stocker les bords d'une intersection
typedef struct {
    int x;
    int dx;
    int ymax;
} Edge;

// Structure pour représenter un pixel
typedef struct {
    int x;
    int y;
} Pixel;

// Fonction de comparaison pour qsort
int compare_edges(const void *a, const void *b) {
    const Edge *edge1 = (const Edge *)a;
    const Edge *edge2 = (const Edge *)b;
    return (edge1->x - edge2->x);
}

void fill_simple_glyph(t_glyph *glyph, t_glyph_ctx *ctx, t_pointi2 position) {
    // Créer un tableau pour stocker les bords
    Edge *edges = malloc(glyph->numpoints * sizeof(Edge));
    if (edges == NULL) {
        // Gérer l'erreur de mémoire
        return;
    }

    // Initialiser les bords en parcourant les contours du glyphe
    int edge_count = 0;
    t_glyph_contour *contour = glyph->contours;
    while (contour != NULL) {
        t_glyph_point *current = get_glyph_point_by_id(glyph, contour->val);
        t_glyph_point *next = get_glyph_point_by_id(glyph, (contour->next != NULL) ? contour->next->val : 0);
        while (current != NULL && next != NULL) {
            // Ajouter un bord horizontal si nécessaire
            if (current->y != next->y) {
                int ymin = (current->y < next->y) ? current->y : next->y;
                int ymax = (current->y > next->y) ? current->y : next->y;
                if (ymax > position.y && ymin < position.y) {
                    edges[edge_count].x = current->x + position.x;
                    edges[edge_count].dx = (next->x - current->x) / (next->y - current->y);
                    edges[edge_count].ymax = ymax;
                    edge_count++;
                }
            }
            // Passer au point suivant
            current = next;
            next = next->next;
        }
        contour = contour->next;
    }

    // Trier les bords selon x
    qsort(edges, edge_count, sizeof(Edge), compare_edges);

    // Initialiser les bords actifs
    Edge *active_edges = malloc(glyph->numpoints * sizeof(Edge));
    if (active_edges == NULL) {
        // Gérer l'erreur de mémoire
        free(edges);
        return;
    }
    int active_count = 0;

    // Parcourir les lignes de scan
    for (int y = position.y; y <= glyph->y_max + position.y; y++) {
        // Ajouter les nouveaux bords actifs
        while (edge_count > 0 && edges[edge_count - 1].ymax == y) {
            active_edges[active_count] = edges[edge_count - 1];
            active_count++;
            edge_count--;
        }

        // Trier les bords actifs selon x
        qsort(active_edges, active_count, sizeof(Edge), compare_edges);

        // Remplir les pixels entre les bords actifs
        for (int i = 0; i < active_count; i += 2) {
            int x_start = active_edges[i].x;
            int x_end = active_edges[i + 1].x;
            for (int x = x_start; x <= x_end; x++) {
                // Remplir le pixel (x, y)
                // Par exemple, vous pouvez utiliser une fonction comme pointc
                // pour dessiner le pixel à la position (x, y)
				printf("x=%d y=%d\n", x, y);
				ft_pixel_put(ctx->pr.image, x, y, ctx->color);
            }
        }

        // Mettre à jour les bords actifs
        int j = 0;
        for (int i = 0; i < active_count; i++) {
            if (active_edges[i].ymax > y + 1) {
                active_edges[j] = active_edges[i];
                j++;
            }
        }
        active_count = j;

        // Mettre à jour les bords actifs pour la prochaine ligne de scan
        for (int i = 0; i < active_count; i++) {
            active_edges[i].x += active_edges[i].dx;
        }
    }

    // Libérer la mémoire
    free(edges);
    free(active_edges);
}

int	draw_glyph(t_true_type_font *this, t_glyph_ctx ctx, t_pointi2 pos)
{
	t_glyph		*glyph;

	glyph = read_glyph(this, ctx.pr.index);
	if (!glyph)
		return (ft_error(FONT_ALLOC_ERROR_MSG), FONT_ALLOC_ERROR);
	ctx.points = ft_calloc(glyph->numpoints * 2 * 10, sizeof(t_pointi2));
	if (!ctx.points)
		return (ft_error(FONT_ALLOC_ERROR_MSG), destroy_glyph(glyph),
			FONT_ALLOC_ERROR);
	ctx.color = 0x00FF00;
	draw_glyph_path(glyph, &ctx, pos);
	// ctx.color = 0xFFFFFF;
	// fill_glyph(glyph, &ctx, pos);
	// fill_simple_glyph(glyph, &ctx, pos);
	free(ctx.points);
	destroy_glyph(glyph);
	return (FONT_NO_ERROR);
}
