#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/common.h"
#include "include/pb_encode.h"
#include "include/pb_decode.h"
#include "include/unionproto.pb.h"
#include "include/ft_socket.h"

const pb_field_t* decode_unionmessage_type(pb_istream_t *stream)
{
	pb_wire_type_t wire_type;
	uint32_t tag;
	bool eof;

	while (pb_decode_tag(stream, &wire_type, &tag, &eof))
	{
		if (wire_type == PB_WT_STRING)
		{
			const pb_field_t *field;
			for (field = UnionMessage_fields; field->tag != 0; field++)
			{
				if (field->tag == tag && (field->type & PB_LTYPE_SUBMESSAGE))
				{
					/* Found our field. */
					return field->ptr;
				}
			}
		}

		/* Wasn't our field.. */
		pb_skip_field(stream, wire_type);
	}

	return NULL;
}
bool encode_unionmessage(pb_ostream_t *stream, const pb_field_t messagetype[], const void *message)
{
	const pb_field_t *field;
	for (field = UnionMessage_fields; field->tag != 0; field++)
	{
		if (field->ptr == messagetype)
		{
			/* This is our field, encode the message using it. */
			if (!pb_encode_tag_for_field(stream, field))
				return false;

			return pb_encode_submessage(stream, messagetype, message);
		}
	}

	/* Didn't find the field for messagetype */
	return false;
}

bool decode_unionmessage_contents(pb_istream_t *stream, const pb_field_t fields[], void *dest_struct)
{
	pb_istream_t substream;
	bool status;
	if (!pb_make_string_substream(stream, &substream))
		return false;

	status = pb_decode(&substream, fields, dest_struct);
	pb_close_string_substream(stream, &substream);
	return status;
}




